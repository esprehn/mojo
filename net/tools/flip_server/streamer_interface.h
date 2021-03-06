// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_FLIP_SERVER_STREAMER_INTERFACE_H_
#define NET_TOOLS_FLIP_SERVER_STREAMER_INTERFACE_H_

#include <string>

#include "base/compiler_specific.h"
#include "net/tools/balsa/balsa_headers.h"
#include "net/tools/balsa/balsa_visitor_interface.h"
#include "net/tools/flip_server/sm_interface.h"

namespace net {

class BalsaFrame;
class FlipAcceptor;
class MemCacheIter;
class SMConnection;
class EpollServer;

class StreamerSM : public BalsaVisitorInterface, public SMInterface {
 public:
  StreamerSM(SMConnection* connection,
             SMInterface* sm_other_interface,
             EpollServer* epoll_server,
             FlipAcceptor* acceptor);
  virtual ~StreamerSM();

  void AddToOutputOrder(const MemCacheIter& mci) {}

  virtual void InitSMInterface(SMInterface* sm_other_interface,
                               int32 server_idx) override;
  virtual void InitSMConnection(SMConnectionPoolInterface* connection_pool,
                                SMInterface* sm_interface,
                                EpollServer* epoll_server,
                                int fd,
                                std::string server_ip,
                                std::string server_port,
                                std::string remote_ip,
                                bool use_ssl) override;

  virtual size_t ProcessReadInput(const char* data, size_t len) override;
  virtual size_t ProcessWriteInput(const char* data, size_t len) override;
  virtual bool MessageFullyRead() const override;
  virtual void SetStreamID(uint32 stream_id) override {}
  virtual bool Error() const override;
  virtual const char* ErrorAsString() const override;
  virtual void Reset() override;
  virtual void ResetForNewInterface(int32 server_idx) override {}
  virtual void ResetForNewConnection() override;
  virtual void Cleanup() override;
  virtual int PostAcceptHook() override;
  virtual void NewStream(uint32 stream_id,
                         uint32 priority,
                         const std::string& filename) override {}
  virtual void SendEOF(uint32 stream_id) override {}
  virtual void SendErrorNotFound(uint32 stream_id) override {}
  virtual void SendOKResponse(uint32 stream_id, std::string output) {}
  virtual size_t SendSynStream(uint32 stream_id,
                               const BalsaHeaders& headers) override;
  virtual size_t SendSynReply(uint32 stream_id,
                              const BalsaHeaders& headers) override;
  virtual void SendDataFrame(uint32 stream_id,
                             const char* data,
                             int64 len,
                             uint32 flags,
                             bool compress) override {}
  virtual void set_is_request() override;
  static std::string forward_ip_header() { return forward_ip_header_; }
  static void set_forward_ip_header(std::string value) {
    forward_ip_header_ = value;
  }

 private:
  void SendEOFImpl(uint32 stream_id) {}
  void SendErrorNotFoundImpl(uint32 stream_id) {}
  void SendOKResponseImpl(uint32 stream_id, std::string* output) {}
  size_t SendSynReplyImpl(uint32 stream_id, const BalsaHeaders& headers) {
    return 0;
  }
  size_t SendSynStreamImpl(uint32 stream_id, const BalsaHeaders& headers) {
    return 0;
  }
  void SendDataFrameImpl(uint32 stream_id,
                         const char* data,
                         int64 len,
                         uint32 flags,
                         bool compress) {}
  virtual void GetOutput() override {}

  virtual void ProcessBodyInput(const char* input, size_t size) override;
  virtual void MessageDone() override;
  virtual void ProcessHeaders(const BalsaHeaders& headers) override;
  virtual void ProcessBodyData(const char* input, size_t size) override {}
  virtual void ProcessHeaderInput(const char* input, size_t size) override {}
  virtual void ProcessTrailerInput(const char* input, size_t size) override {}
  virtual void ProcessRequestFirstLine(const char* line_input,
                                       size_t line_length,
                                       const char* method_input,
                                       size_t method_length,
                                       const char* request_uri_input,
                                       size_t request_uri_length,
                                       const char* version_input,
                                       size_t version_length) override {}
  virtual void ProcessResponseFirstLine(const char* line_input,
                                        size_t line_length,
                                        const char* version_input,
                                        size_t version_length,
                                        const char* status_input,
                                        size_t status_length,
                                        const char* reason_input,
                                        size_t reason_length) override {}
  virtual void ProcessChunkLength(size_t chunk_length) override {}
  virtual void ProcessChunkExtensions(const char* input, size_t size) override {
  }
  virtual void HeaderDone() override {}
  virtual void HandleHeaderError(BalsaFrame* framer) override;
  virtual void HandleHeaderWarning(BalsaFrame* framer) override {}
  virtual void HandleChunkingError(BalsaFrame* framer) override;
  virtual void HandleBodyError(BalsaFrame* framer) override;
  void HandleError();

  SMConnection* connection_;
  SMInterface* sm_other_interface_;
  EpollServer* epoll_server_;
  FlipAcceptor* acceptor_;
  bool is_request_;
  BalsaFrame* http_framer_;
  BalsaHeaders headers_;
  static std::string forward_ip_header_;
};

}  // namespace net

#endif  // NET_TOOLS_FLIP_SERVER_STREAMER_INTERFACE_H_
