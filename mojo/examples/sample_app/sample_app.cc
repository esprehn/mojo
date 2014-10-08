// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <string>

#include "base/bind.h"
#include "base/memory/weak_ptr.h"
#include "mojo/examples/sample_app/gles2_client_impl.h"
#include "mojo/public/c/system/main.h"
#include "mojo/public/cpp/application/application_connection.h"
#include "mojo/public/cpp/application/application_delegate.h"
#include "mojo/public/cpp/application/application_impl.h"
#include "mojo/public/cpp/application/application_runner.h"
#include "mojo/public/cpp/system/core.h"
#include "mojo/public/cpp/system/macros.h"
#include "mojo/public/cpp/utility/run_loop.h"
#include "mojo/services/public/interfaces/gpu/gpu.mojom.h"
#include "mojo/services/public/interfaces/native_viewport/native_viewport.mojom.h"

namespace examples {

class SampleApp : public mojo::ApplicationDelegate,
                  public mojo::NativeViewportClient {
 public:
  SampleApp() : weak_factory_(this) {}

  virtual ~SampleApp() {
    // TODO(darin): Fix shutdown so we don't need to leak this.
    MOJO_ALLOW_UNUSED GLES2ClientImpl* leaked = gles2_client_.release();
  }

  virtual void Initialize(mojo::ApplicationImpl* app) override {
    app->ConnectToService("mojo:mojo_native_viewport_service", &viewport_);
    viewport_.set_client(this);

    // TODO(jamesr): Should be mojo:mojo_gpu_service
    app->ConnectToService("mojo:mojo_native_viewport_service", &gpu_service_);

    mojo::SizePtr size(mojo::Size::New());
    size->width = 800;
    size->height = 600;
    viewport_->Create(size.Pass(),
                      base::Bind(&SampleApp::OnCreatedNativeViewport,
                                 weak_factory_.GetWeakPtr()));
    viewport_->Show();
  }

  virtual void OnDestroyed() override { mojo::RunLoop::current()->Quit(); }

  virtual void OnSizeChanged(mojo::SizePtr size) override {
    assert(size);
    if (gles2_client_)
      gles2_client_->SetSize(*size);
  }

  virtual void OnEvent(mojo::EventPtr event,
                       const mojo::Callback<void()>& callback) override {
    assert(event);
    if (event->location_data && event->location_data->in_view_location)
      gles2_client_->HandleInputEvent(*event);
    callback.Run();
  }

 private:
  void OnCreatedNativeViewport(uint64_t native_viewport_id) {
    mojo::SizePtr size = mojo::Size::New();
    size->width = 800;
    size->height = 600;
    mojo::CommandBufferPtr command_buffer;
    // TODO(jamesr): Output to a surface instead.
    gpu_service_->CreateOnscreenGLES2Context(
        native_viewport_id, size.Pass(), GetProxy(&command_buffer));
    gles2_client_.reset(new GLES2ClientImpl(command_buffer.Pass()));
  }

  scoped_ptr<GLES2ClientImpl> gles2_client_;
  mojo::NativeViewportPtr viewport_;
  mojo::GpuPtr gpu_service_;
  base::WeakPtrFactory<SampleApp> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(SampleApp);
};

}  // namespace examples

MojoResult MojoMain(MojoHandle shell_handle) {
  mojo::ApplicationRunner runner(new examples::SampleApp);
  return runner.Run(shell_handle);
}
