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

#include "config.h"
#include "core/dom/ElementRareData.h"

#include "core/rendering/style/RenderStyle.h"

namespace blink {

struct SameSizeAsElementRareData : NodeRareData {
    short index;
    LayoutSize sizeForResizing;
    IntSize scrollOffset;
    void* pointers[9];
};

CSSStyleDeclaration& ElementRareData::ensureInlineCSSStyleDeclaration(Element* ownerElement)
{
    if (!m_cssomWrapper)
        m_cssomWrapper = adoptPtr(new InlineCSSStyleDeclaration(ownerElement));
    return *m_cssomWrapper;
}

Vector<RefPtr<Attr> >& ElementRareData::ensureAttrNodeList()
{
    if (!m_attrNodeList)
        m_attrNodeList = adoptPtr(new Vector<RefPtr<Attr> >());
    return *m_attrNodeList;
}

void ElementRareData::traceAfterDispatch(Visitor* visitor)
{
    visitor->trace(m_classList);
    visitor->trace(m_shadow);
    visitor->trace(m_attributeMap);
#if ENABLE(OILPAN)
    visitor->trace(m_attrNodeList);
#endif
    visitor->trace(m_inputMethodContext);
    visitor->trace(m_activeAnimations);
    visitor->trace(m_cssomWrapper);
    NodeRareData::traceAfterDispatch(visitor);
}

COMPILE_ASSERT(sizeof(ElementRareData) == sizeof(SameSizeAsElementRareData), ElementRareDataShouldStaySmall);

} // namespace blink
