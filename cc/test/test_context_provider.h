// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_TEST_CONTEXT_PROVIDER_H_
#define CC_TEST_TEST_CONTEXT_PROVIDER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "cc/output/context_provider.h"
#include "cc/test/test_context_support.h"
#include "gpu/command_buffer/client/gles2_interface_stub.h"

namespace cc {
class TestWebGraphicsContext3D;
class TestGLES2Interface;

class TestContextProvider : public ContextProvider {
 public:
  typedef base::Callback<scoped_ptr<TestWebGraphicsContext3D>(void)>
    CreateCallback;

  static scoped_refptr<TestContextProvider> Create();
  static scoped_refptr<TestContextProvider> Create(
      scoped_ptr<TestWebGraphicsContext3D> context);

  virtual bool BindToCurrentThread() override;
  virtual Capabilities ContextCapabilities() override;
  virtual gpu::gles2::GLES2Interface* ContextGL() override;
  virtual gpu::ContextSupport* ContextSupport() override;
  virtual class GrContext* GrContext() override;
  virtual bool IsContextLost() override;
  virtual void VerifyContexts() override;
  virtual void DeleteCachedResources() override;
  virtual bool DestroyedOnMainThread() override;
  virtual void SetLostContextCallback(const LostContextCallback& cb) override;
  virtual void SetMemoryPolicyChangedCallback(
      const MemoryPolicyChangedCallback& cb) override;

  TestWebGraphicsContext3D* TestContext3d();

  // This returns the TestWebGraphicsContext3D but is valid to call
  // before the context is bound to a thread. This is needed to set up
  // state on the test context before binding. Don't call
  // InitializeOnCurrentThread on the context returned from this method.
  TestWebGraphicsContext3D* UnboundTestContext3d();

  TestContextSupport* support() { return &support_; }

  void SetMemoryAllocation(const ManagedMemoryPolicy& policy);

  void SetMaxTransferBufferUsageBytes(size_t max_transfer_buffer_usage_bytes);

 protected:
  explicit TestContextProvider(scoped_ptr<TestWebGraphicsContext3D> context);
  virtual ~TestContextProvider();

 private:
  void OnLostContext();

  TestContextSupport support_;

  scoped_ptr<TestWebGraphicsContext3D> context3d_;
  scoped_ptr<TestGLES2Interface> context_gl_;
  bool bound_;

  base::ThreadChecker main_thread_checker_;
  base::ThreadChecker context_thread_checker_;

  base::Lock destroyed_lock_;
  bool destroyed_;

  LostContextCallback lost_context_callback_;
  MemoryPolicyChangedCallback memory_policy_changed_callback_;

  base::WeakPtrFactory<TestContextProvider> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(TestContextProvider);
};

}  // namespace cc

#endif  // CC_TEST_TEST_CONTEXT_PROVIDER_H_

