// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/quic_headers_stream.h"

#include "net/quic/quic_session.h"

using base::StringPiece;

namespace net {

namespace {

const QuicStreamId kInvalidStreamId = 0;

}  // namespace

// A SpdyFramer visitor which passed SYN_STREAM and SYN_REPLY frames to
// the QuicDataStream, and closes the connection if any unexpected frames
// are received.
class QuicHeadersStream::SpdyFramerVisitor
    : public SpdyFramerVisitorInterface,
      public SpdyFramerDebugVisitorInterface {
 public:
  explicit SpdyFramerVisitor(QuicHeadersStream* stream) : stream_(stream) {}

  // SpdyFramerVisitorInterface implementation
  void OnSynStream(SpdyStreamId stream_id,
                   SpdyStreamId associated_stream_id,
                   SpdyPriority priority,
                   bool fin,
                   bool unidirectional) override {
    if (!stream_->IsConnected()) {
      return;
    }

    if (associated_stream_id != 0) {
      CloseConnection("associated_stream_id != 0");
      return;
    }

    if (unidirectional != 0) {
      CloseConnection("unidirectional != 0");
      return;
    }

    stream_->OnSynStream(stream_id, priority, fin);
  }

  void OnSynReply(SpdyStreamId stream_id, bool fin) override {
    if (!stream_->IsConnected()) {
      return;
    }

    stream_->OnSynReply(stream_id, fin);
  }

  bool OnControlFrameHeaderData(SpdyStreamId stream_id,
                                const char* header_data,
                                size_t len) override {
    if (!stream_->IsConnected()) {
      return false;
    }
    stream_->OnControlFrameHeaderData(stream_id, header_data, len);
    return true;
  }

  void OnStreamFrameData(SpdyStreamId stream_id,
                         const char* data,
                         size_t len,
                         bool fin) override {
    if (fin && len == 0) {
      // The framer invokes OnStreamFrameData with zero-length data and
      // fin = true after processing a SYN_STREAM or SYN_REPLY frame
      // that had the fin bit set.
      return;
    }
    CloseConnection("SPDY DATA frame received.");
  }

  void OnError(SpdyFramer* framer) override {
    CloseConnection("SPDY framing error.");
  }

  void OnDataFrameHeader(SpdyStreamId stream_id,
                         size_t length,
                         bool fin) override {
    CloseConnection("SPDY DATA frame received.");
  }

  void OnRstStream(SpdyStreamId stream_id,
                   SpdyRstStreamStatus status) override {
    CloseConnection("SPDY RST_STREAM frame received.");
  }

  void OnSetting(SpdySettingsIds id, uint8 flags, uint32 value) override {
    CloseConnection("SPDY SETTINGS frame received.");
  }

  void OnSettingsAck() override {
    CloseConnection("SPDY SETTINGS frame received.");
  }

  void OnSettingsEnd() override {
    CloseConnection("SPDY SETTINGS frame received.");
  }

  void OnPing(SpdyPingId unique_id, bool is_ack) override {
    CloseConnection("SPDY PING frame received.");
  }

  void OnGoAway(SpdyStreamId last_accepted_stream_id,
                SpdyGoAwayStatus status) override {
    CloseConnection("SPDY GOAWAY frame received.");
  }

  void OnHeaders(SpdyStreamId stream_id, bool fin, bool end) override {
    CloseConnection("SPDY HEADERS frame received.");
  }

  void OnWindowUpdate(SpdyStreamId stream_id,
                      uint32 delta_window_size) override {
    CloseConnection("SPDY WINDOW_UPDATE frame received.");
  }

  void OnPushPromise(SpdyStreamId stream_id,
                     SpdyStreamId promised_stream_id,
                     bool end) override {
    LOG(DFATAL) << "PUSH_PROMISE frame received from a SPDY/3 framer";
    CloseConnection("SPDY PUSH_PROMISE frame received.");
  }

  void OnContinuation(SpdyStreamId stream_id, bool end) override {
    CloseConnection("SPDY CONTINUATION frame received.");
  }

  bool OnUnknownFrame(SpdyStreamId stream_id, int frame_type) override {
    CloseConnection("SPDY unknown frame received.");
    return false;
  }

  // SpdyFramerDebugVisitorInterface implementation
  void OnSendCompressedFrame(SpdyStreamId stream_id,
                             SpdyFrameType type,
                             size_t payload_len,
                             size_t frame_len) override {}

  void OnReceiveCompressedFrame(SpdyStreamId stream_id,
                                SpdyFrameType type,
                                size_t frame_len) override {
    if (stream_->IsConnected()) {
      stream_->OnCompressedFrameSize(frame_len);
    }
  }

 private:
  void CloseConnection(const string& details) {
    if (stream_->IsConnected()) {
      stream_->CloseConnectionWithDetails(
          QUIC_INVALID_HEADERS_STREAM_DATA, details);
    }
  }

 private:
  QuicHeadersStream* stream_;

  DISALLOW_COPY_AND_ASSIGN(SpdyFramerVisitor);
};

QuicHeadersStream::QuicHeadersStream(QuicSession* session)
    : ReliableQuicStream(kHeadersStreamId, session),
      stream_id_(kInvalidStreamId),
      fin_(false),
      frame_len_(0),
      spdy_framer_(SPDY3),
      spdy_framer_visitor_(new SpdyFramerVisitor(this)) {
  spdy_framer_.set_visitor(spdy_framer_visitor_.get());
  spdy_framer_.set_debug_visitor(spdy_framer_visitor_.get());
  if (version() < QUIC_VERSION_21) {
    // Prior to QUIC_VERSION_21 the headers stream is not subject to any flow
    // control.
    DisableFlowControl();
  }
  // The headers stream is exempt from connection level flow control.
  DisableConnectionFlowControlForThisStream();
}

QuicHeadersStream::~QuicHeadersStream() {}

size_t QuicHeadersStream::WriteHeaders(
    QuicStreamId stream_id,
    const SpdyHeaderBlock& headers,
    bool fin,
    QuicAckNotifier::DelegateInterface* ack_notifier_delegate) {
  scoped_ptr<SpdySerializedFrame> frame;
  if (session()->is_server()) {
    SpdySynReplyIR syn_reply(stream_id);
    syn_reply.set_name_value_block(headers);
    syn_reply.set_fin(fin);
    frame.reset(spdy_framer_.SerializeFrame(syn_reply));
  } else {
    SpdySynStreamIR syn_stream(stream_id);
    syn_stream.set_name_value_block(headers);
    syn_stream.set_fin(fin);
    frame.reset(spdy_framer_.SerializeFrame(syn_stream));
  }
  WriteOrBufferData(StringPiece(frame->data(), frame->size()), false,
                    ack_notifier_delegate);
  return frame->size();
}

uint32 QuicHeadersStream::ProcessRawData(const char* data,
                                         uint32 data_len) {
  return spdy_framer_.ProcessInput(data, data_len);
}

QuicPriority QuicHeadersStream::EffectivePriority() const { return 0; }

void QuicHeadersStream::OnSynStream(SpdyStreamId stream_id,
                                    SpdyPriority priority,
                                    bool fin) {
  if (!session()->is_server()) {
    CloseConnectionWithDetails(
        QUIC_INVALID_HEADERS_STREAM_DATA,
        "SPDY SYN_STREAM frame received at the client");
    return;
  }
  DCHECK_EQ(kInvalidStreamId, stream_id_);
  stream_id_ = stream_id;
  fin_ = fin;
  session()->OnStreamHeadersPriority(stream_id, priority);
}

void QuicHeadersStream::OnSynReply(SpdyStreamId stream_id, bool fin) {
  if (session()->is_server()) {
    CloseConnectionWithDetails(
        QUIC_INVALID_HEADERS_STREAM_DATA,
        "SPDY SYN_REPLY frame received at the server");
    return;
  }
  DCHECK_EQ(kInvalidStreamId, stream_id_);
  stream_id_ = stream_id;
  fin_ = fin;
}

void QuicHeadersStream::OnControlFrameHeaderData(SpdyStreamId stream_id,
                                                 const char* header_data,
                                                 size_t len) {
  DCHECK_EQ(stream_id_, stream_id);
  if (len == 0) {
    DCHECK_NE(0u, stream_id_);
    DCHECK_NE(0u, frame_len_);
    session()->OnStreamHeadersComplete(stream_id_, fin_, frame_len_);
    // Reset state for the next frame.
    stream_id_ = kInvalidStreamId;
    fin_ = false;
    frame_len_ = 0;
  } else {
    session()->OnStreamHeaders(stream_id_, StringPiece(header_data, len));
  }
}

void QuicHeadersStream::OnCompressedFrameSize(size_t frame_len) {
  DCHECK_EQ(kInvalidStreamId, stream_id_);
  DCHECK_EQ(0u, frame_len_);
  frame_len_ = frame_len;
}

bool QuicHeadersStream::IsConnected() {
  return session()->connection()->connected();
}

}  // namespace net
