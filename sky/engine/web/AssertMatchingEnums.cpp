/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Use this file to assert that various public API enum values continue
// matching blink defined enum values.

#include "config.h"

#include "bindings/core/v8/SerializedScriptValue.h"
#include "core/dom/DocumentMarker.h"
#include "core/dom/ExceptionCode.h"
#include "core/editing/TextAffinity.h"
#include "core/frame/ConsoleTypes.h"
#include "core/frame/Settings.h"
#include "core/html/HTMLMediaElement.h"
#include "core/loader/FrameLoaderTypes.h"
#include "core/loader/NavigationPolicy.h"
#include "core/page/PageVisibilityState.h"
#include "core/rendering/style/RenderStyleConstants.h"
#include "platform/Cursor.h"
#include "platform/PlatformMouseEvent.h"
#include "platform/fonts/FontDescription.h"
#include "platform/fonts/FontSmoothingMode.h"
#include "platform/graphics/filters/FilterOperation.h"
#include "platform/graphics/media/MediaPlayer.h"
#include "platform/network/ResourceLoadPriority.h"
#include "platform/network/ResourceResponse.h"
#include "platform/scroll/ScrollTypes.h"
#include "platform/text/TextChecking.h"
#include "platform/text/TextDecoration.h"
#include "platform/weborigin/ReferrerPolicy.h"
#include "public/platform/WebClipboard.h"
#include "public/platform/WebCursorInfo.h"
#include "public/platform/WebMediaPlayer.h"
#include "public/platform/WebMediaPlayerClient.h"
#include "public/platform/WebMediaSource.h"
#include "public/platform/WebReferrerPolicy.h"
#include "public/platform/WebScrollbar.h"
#include "public/platform/WebScrollbarBehavior.h"
#include "public/platform/WebURLRequest.h"
#include "public/platform/WebURLResponse.h"
#include "public/web/WebConsoleMessage.h"
#include "public/web/WebFontDescription.h"
#include "public/web/WebInputEvent.h"
#include "public/web/WebNavigationPolicy.h"
#include "public/web/WebNavigatorContentUtilsClient.h"
#include "public/web/WebNode.h"
#include "public/web/WebPageVisibilityState.h"
#include "public/web/WebSerializedScriptValueVersion.h"
#include "public/web/WebSettings.h"
#include "public/web/WebTextAffinity.h"
#include "public/web/WebTextCheckingResult.h"
#include "public/web/WebTextCheckingType.h"
#include "public/web/WebTextDecorationType.h"
#include "public/web/WebTouchAction.h"
#include "public/web/WebView.h"
#include "wtf/Assertions.h"
#include "wtf/text/StringImpl.h"

namespace blink {

#define COMPILE_ASSERT_MATCHING_ENUM(public_name, core_name) \
    COMPILE_ASSERT(int(public_name) == int(core_name), mismatching_enums)

#define COMPILE_ASSERT_MATCHING_UINT64(public_name, core_name) \
    COMPILE_ASSERT(public_name == core_name, mismatching_enums)

COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypePointer, Cursor::Pointer);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeCross, Cursor::Cross);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeHand, Cursor::Hand);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeIBeam, Cursor::IBeam);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeWait, Cursor::Wait);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeHelp, Cursor::Help);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeEastResize, Cursor::EastResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthResize, Cursor::NorthResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthEastResize, Cursor::NorthEastResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthWestResize, Cursor::NorthWestResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeSouthResize, Cursor::SouthResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeSouthEastResize, Cursor::SouthEastResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeSouthWestResize, Cursor::SouthWestResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeWestResize, Cursor::WestResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthSouthResize, Cursor::NorthSouthResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeEastWestResize, Cursor::EastWestResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthEastSouthWestResize, Cursor::NorthEastSouthWestResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthWestSouthEastResize, Cursor::NorthWestSouthEastResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeColumnResize, Cursor::ColumnResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeRowResize, Cursor::RowResize);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeMiddlePanning, Cursor::MiddlePanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeEastPanning, Cursor::EastPanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthPanning, Cursor::NorthPanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthEastPanning, Cursor::NorthEastPanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNorthWestPanning, Cursor::NorthWestPanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeSouthPanning, Cursor::SouthPanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeSouthEastPanning, Cursor::SouthEastPanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeSouthWestPanning, Cursor::SouthWestPanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeWestPanning, Cursor::WestPanning);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeMove, Cursor::Move);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeVerticalText, Cursor::VerticalText);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeCell, Cursor::Cell);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeContextMenu, Cursor::ContextMenu);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeAlias, Cursor::Alias);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeProgress, Cursor::Progress);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNoDrop, Cursor::NoDrop);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeCopy, Cursor::Copy);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNone, Cursor::None);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeNotAllowed, Cursor::NotAllowed);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeZoomIn, Cursor::ZoomIn);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeZoomOut, Cursor::ZoomOut);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeGrab, Cursor::Grab);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeGrabbing, Cursor::Grabbing);
COMPILE_ASSERT_MATCHING_ENUM(WebCursorInfo::TypeCustom, Cursor::Custom);

COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::GenericFamilyNone, FontDescription::NoFamily);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::GenericFamilyStandard, FontDescription::StandardFamily);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::GenericFamilySerif, FontDescription::SerifFamily);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::GenericFamilySansSerif, FontDescription::SansSerifFamily);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::GenericFamilyMonospace, FontDescription::MonospaceFamily);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::GenericFamilyCursive, FontDescription::CursiveFamily);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::GenericFamilyFantasy, FontDescription::FantasyFamily);

COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::SmoothingAuto, AutoSmoothing);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::SmoothingNone, NoSmoothing);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::SmoothingGrayscale, Antialiased);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::SmoothingSubpixel, SubpixelAntialiased);

COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight100, FontWeight100);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight200, FontWeight200);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight300, FontWeight300);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight400, FontWeight400);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight500, FontWeight500);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight600, FontWeight600);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight700, FontWeight700);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight800, FontWeight800);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::Weight900, FontWeight900);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::WeightNormal, FontWeightNormal);
COMPILE_ASSERT_MATCHING_ENUM(WebFontDescription::WeightBold, FontWeightBold);

COMPILE_ASSERT_MATCHING_ENUM(WebNode::ElementNode, Node::ELEMENT_NODE);
COMPILE_ASSERT_MATCHING_ENUM(WebNode::AttributeNode, Node::ATTRIBUTE_NODE);
COMPILE_ASSERT_MATCHING_ENUM(WebNode::TextNode, Node::TEXT_NODE);
COMPILE_ASSERT_MATCHING_ENUM(WebNode::DocumentNode, Node::DOCUMENT_NODE);
COMPILE_ASSERT_MATCHING_ENUM(WebNode::DocumentFragmentNode, Node::DOCUMENT_FRAGMENT_NODE);

COMPILE_ASSERT_MATCHING_ENUM(WebMediaPlayer::ReadyStateHaveNothing, HTMLMediaElement::HAVE_NOTHING);
COMPILE_ASSERT_MATCHING_ENUM(WebMediaPlayer::ReadyStateHaveMetadata, HTMLMediaElement::HAVE_METADATA);
COMPILE_ASSERT_MATCHING_ENUM(WebMediaPlayer::ReadyStateHaveCurrentData, HTMLMediaElement::HAVE_CURRENT_DATA);
COMPILE_ASSERT_MATCHING_ENUM(WebMediaPlayer::ReadyStateHaveFutureData, HTMLMediaElement::HAVE_FUTURE_DATA);
COMPILE_ASSERT_MATCHING_ENUM(WebMediaPlayer::ReadyStateHaveEnoughData, HTMLMediaElement::HAVE_ENOUGH_DATA);

COMPILE_ASSERT_MATCHING_ENUM(WebMediaPlayer::PreloadNone, MediaPlayer::None);
COMPILE_ASSERT_MATCHING_ENUM(WebMediaPlayer::PreloadMetaData, MediaPlayer::MetaData);
COMPILE_ASSERT_MATCHING_ENUM(WebMediaPlayer::PreloadAuto, MediaPlayer::Auto);

COMPILE_ASSERT_MATCHING_ENUM(WebMouseEvent::ButtonNone, NoButton);
COMPILE_ASSERT_MATCHING_ENUM(WebMouseEvent::ButtonLeft, LeftButton);
COMPILE_ASSERT_MATCHING_ENUM(WebMouseEvent::ButtonMiddle, MiddleButton);
COMPILE_ASSERT_MATCHING_ENUM(WebMouseEvent::ButtonRight, RightButton);

COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::Horizontal, HorizontalScrollbar);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::Vertical, VerticalScrollbar);

COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::ScrollByLine, ScrollByLine);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::ScrollByPage, ScrollByPage);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::ScrollByDocument, ScrollByDocument);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::ScrollByPixel, ScrollByPixel);

COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::NoPart, NoPart);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::BackTrackPart, BackTrackPart);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::ThumbPart, ThumbPart);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::ForwardTrackPart, ForwardTrackPart);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::ScrollbarOverlayStyleDefault, ScrollbarOverlayStyleDefault);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbar::ScrollbarOverlayStyleLight, ScrollbarOverlayStyleLight);

COMPILE_ASSERT_MATCHING_ENUM(WebScrollbarBehavior::ButtonNone, NoButton);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbarBehavior::ButtonLeft, LeftButton);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbarBehavior::ButtonMiddle, MiddleButton);
COMPILE_ASSERT_MATCHING_ENUM(WebScrollbarBehavior::ButtonRight, RightButton);

COMPILE_ASSERT_MATCHING_ENUM(WebTextAffinityUpstream, UPSTREAM);
COMPILE_ASSERT_MATCHING_ENUM(WebTextAffinityDownstream, DOWNSTREAM);

COMPILE_ASSERT_MATCHING_ENUM(WebTextCheckingTypeSpelling, TextCheckingTypeSpelling);
COMPILE_ASSERT_MATCHING_ENUM(WebTextCheckingTypeGrammar, TextCheckingTypeGrammar);

// TODO(rouslan): Remove these comparisons between text-checking and text-decoration enum values after removing the
// deprecated constructor WebTextCheckingResult(WebTextCheckingType).
COMPILE_ASSERT_MATCHING_ENUM(WebTextCheckingTypeSpelling, TextDecorationTypeSpelling);
COMPILE_ASSERT_MATCHING_ENUM(WebTextCheckingTypeGrammar, TextDecorationTypeGrammar);

COMPILE_ASSERT_MATCHING_ENUM(WebTextDecorationTypeSpelling, TextDecorationTypeSpelling);
COMPILE_ASSERT_MATCHING_ENUM(WebTextDecorationTypeGrammar, TextDecorationTypeGrammar);
COMPILE_ASSERT_MATCHING_ENUM(WebTextDecorationTypeInvisibleSpellcheck, TextDecorationTypeInvisibleSpellcheck);

COMPILE_ASSERT_MATCHING_ENUM(WebPageVisibilityStateVisible, PageVisibilityStateVisible);
COMPILE_ASSERT_MATCHING_ENUM(WebPageVisibilityStateHidden, PageVisibilityStateHidden);

COMPILE_ASSERT_MATCHING_ENUM(WebReferrerPolicyAlways, ReferrerPolicyAlways);
COMPILE_ASSERT_MATCHING_ENUM(WebReferrerPolicyDefault, ReferrerPolicyDefault);
COMPILE_ASSERT_MATCHING_ENUM(WebReferrerPolicyNever, ReferrerPolicyNever);
COMPILE_ASSERT_MATCHING_ENUM(WebReferrerPolicyOrigin, ReferrerPolicyOrigin);

COMPILE_ASSERT_MATCHING_ENUM(WebURLResponse::Unknown, ResourceResponse::Unknown);
COMPILE_ASSERT_MATCHING_ENUM(WebURLResponse::HTTP_0_9, ResourceResponse::HTTP_0_9);
COMPILE_ASSERT_MATCHING_ENUM(WebURLResponse::HTTP_1_0, ResourceResponse::HTTP_1_0);
COMPILE_ASSERT_MATCHING_ENUM(WebURLResponse::HTTP_1_1, ResourceResponse::HTTP_1_1);

COMPILE_ASSERT_MATCHING_ENUM(WebURLRequest::PriorityUnresolved, ResourceLoadPriorityUnresolved);
COMPILE_ASSERT_MATCHING_ENUM(WebURLRequest::PriorityVeryLow, ResourceLoadPriorityVeryLow);
COMPILE_ASSERT_MATCHING_ENUM(WebURLRequest::PriorityLow, ResourceLoadPriorityLow);
COMPILE_ASSERT_MATCHING_ENUM(WebURLRequest::PriorityMedium, ResourceLoadPriorityMedium);
COMPILE_ASSERT_MATCHING_ENUM(WebURLRequest::PriorityHigh, ResourceLoadPriorityHigh);
COMPILE_ASSERT_MATCHING_ENUM(WebURLRequest::PriorityVeryHigh, ResourceLoadPriorityVeryHigh);

COMPILE_ASSERT_MATCHING_ENUM(WebNavigationPolicyIgnore, NavigationPolicyIgnore);
COMPILE_ASSERT_MATCHING_ENUM(WebNavigationPolicyDownload, NavigationPolicyDownload);
COMPILE_ASSERT_MATCHING_ENUM(WebNavigationPolicyDownloadTo, NavigationPolicyDownloadTo);
COMPILE_ASSERT_MATCHING_ENUM(WebNavigationPolicyCurrentTab, NavigationPolicyCurrentTab);
COMPILE_ASSERT_MATCHING_ENUM(WebNavigationPolicyNewBackgroundTab, NavigationPolicyNewBackgroundTab);
COMPILE_ASSERT_MATCHING_ENUM(WebNavigationPolicyNewForegroundTab, NavigationPolicyNewForegroundTab);
COMPILE_ASSERT_MATCHING_ENUM(WebNavigationPolicyNewWindow, NavigationPolicyNewWindow);
COMPILE_ASSERT_MATCHING_ENUM(WebNavigationPolicyNewPopup, NavigationPolicyNewPopup);

COMPILE_ASSERT_MATCHING_ENUM(WebConsoleMessage::LevelDebug, DebugMessageLevel);
COMPILE_ASSERT_MATCHING_ENUM(WebConsoleMessage::LevelLog, LogMessageLevel);
COMPILE_ASSERT_MATCHING_ENUM(WebConsoleMessage::LevelWarning, WarningMessageLevel);
COMPILE_ASSERT_MATCHING_ENUM(WebConsoleMessage::LevelError, ErrorMessageLevel);
COMPILE_ASSERT_MATCHING_ENUM(WebConsoleMessage::LevelInfo, InfoMessageLevel);

COMPILE_ASSERT_MATCHING_ENUM(WebTouchActionNone, TouchActionNone);
COMPILE_ASSERT_MATCHING_ENUM(WebTouchActionAuto, TouchActionAuto);
COMPILE_ASSERT_MATCHING_ENUM(WebTouchActionPanX, TouchActionPanX);
COMPILE_ASSERT_MATCHING_ENUM(WebTouchActionPanY, TouchActionPanY);
COMPILE_ASSERT_MATCHING_ENUM(WebTouchActionPinchZoom, TouchActionPinchZoom);

COMPILE_ASSERT_MATCHING_ENUM(WebSettings::V8CacheOptionsOff, V8CacheOptionsOff);
COMPILE_ASSERT_MATCHING_ENUM(WebSettings::V8CacheOptionsParse, V8CacheOptionsParse);
COMPILE_ASSERT_MATCHING_ENUM(WebSettings::V8CacheOptionsCode, V8CacheOptionsCode);

COMPILE_ASSERT_MATCHING_ENUM(WebSettings::PointerTypeNone, PointerTypeNone);
COMPILE_ASSERT_MATCHING_ENUM(WebSettings::PointerTypeCoarse, PointerTypeCoarse);
COMPILE_ASSERT_MATCHING_ENUM(WebSettings::PointerTypeFine, PointerTypeFine);
COMPILE_ASSERT_MATCHING_ENUM(WebSettings::HoverTypeNone, HoverTypeNone);
COMPILE_ASSERT_MATCHING_ENUM(WebSettings::HoverTypeOnDemand, HoverTypeOnDemand);
COMPILE_ASSERT_MATCHING_ENUM(WebSettings::HoverTypeHover, HoverTypeHover);

COMPILE_ASSERT_MATCHING_UINT64(kSerializedScriptValueVersion, SerializedScriptValue::wireFormatVersion);

} // namespace blink
