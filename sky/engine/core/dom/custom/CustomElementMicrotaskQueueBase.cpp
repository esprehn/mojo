// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "core/dom/custom/CustomElementMicrotaskQueueBase.h"

#include "core/dom/custom/CustomElementProcessingStack.h"

namespace blink {

void CustomElementMicrotaskQueueBase::dispatch()
{
    ASSERT(!m_inDispatch);
    m_inDispatch = true;
    doDispatch();
    m_inDispatch = false;
}

void CustomElementMicrotaskQueueBase::trace(Visitor* visitor)
{
    visitor->trace(m_queue);
}

} // namespace blink
