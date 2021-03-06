// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/apps/js/application_delegate_impl.h"

#include "gin/array_buffer.h"
#include "gin/public/isolate_holder.h"
#include "mojo/apps/js/js_app.h"
#include "mojo/public/cpp/application/application_impl.h"

namespace mojo {
namespace apps {

ApplicationDelegateImpl::ApplicationDelegateImpl()
    : application_impl_(nullptr) {
}

void ApplicationDelegateImpl::Initialize(ApplicationImpl* app) {
  application_impl_ = app;
  gin::IsolateHolder::Initialize(gin::IsolateHolder::kStrictMode,
                                 gin::ArrayBufferAllocator::SharedInstance());
}

ApplicationDelegateImpl::~ApplicationDelegateImpl() {
}

void ApplicationDelegateImpl::StartJSApp(scoped_ptr<JSApp> app_ptr) {
  JSApp *app = app_ptr.release();
  app_vector_.push_back(app);
  // TODO(hansmuller): deal with the Start() return value.
  app->Start();
}

void ApplicationDelegateImpl::QuitJSApp(JSApp* app) {
  AppVector::iterator itr =
      std::find(app_vector_.begin(), app_vector_.end(), app);
  if (itr != app_vector_.end())
    app_vector_.erase(itr);
  if (app_vector_.empty())
    base::MessageLoop::current()->QuitNow();
}

void ApplicationDelegateImpl::ConnectToApplication(
    const std::string& application_url,
    InterfaceRequest<ServiceProvider> request) {
  CHECK(application_impl_);
  application_impl_->shell()->ConnectToApplication(application_url,
                                                   request.Pass());
}

}  // namespace apps
}  // namespace mojo
