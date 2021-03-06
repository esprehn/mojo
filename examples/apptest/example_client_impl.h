// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_EXAMPLES_TEST_EXAMPLE_CLIENT_IMPL_H_
#define MOJO_EXAMPLES_TEST_EXAMPLE_CLIENT_IMPL_H_

#include "examples/apptest/example_service.mojom.h"
#include "mojo/public/cpp/system/macros.h"

namespace mojo {

class ExampleClientImpl : public InterfaceImpl<ExampleClient> {
 public:
  ExampleClientImpl();
  virtual ~ExampleClientImpl();

  int16_t last_pong_value() const { return last_pong_value_; }

  // InterfaceImpl<ExampleClient> overrides.
  virtual void Pong(uint16_t pong_value) override;

 private:
  int16_t last_pong_value_;
  MOJO_DISALLOW_COPY_AND_ASSIGN(ExampleClientImpl);
};

}  // namespace mojo

#endif  // MOJO_EXAMPLES_TEST_EXAMPLE_CLIENT_IMPL_H_
