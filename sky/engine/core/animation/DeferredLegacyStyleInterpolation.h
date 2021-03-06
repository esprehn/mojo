// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DeferredLegacyStyleInterpolation_h
#define DeferredLegacyStyleInterpolation_h

#include "core/animation/StyleInterpolation.h"
#include "core/css/CSSValue.h"

namespace blink {

class CSSBasicShape;
class CSSImageValue;
class CSSPrimitiveValue;
class CSSShadowValue;
class CSSValueList;

class DeferredLegacyStyleInterpolation : public StyleInterpolation {
public:
    static PassRefPtr<DeferredLegacyStyleInterpolation> create(PassRefPtr<CSSValue> start, PassRefPtr<CSSValue> end, CSSPropertyID id)
    {
        return adoptRef(new DeferredLegacyStyleInterpolation(start, end, id));
    }

    virtual void apply(StyleResolverState&) const override;

    virtual void trace(Visitor*) override;

    static bool interpolationRequiresStyleResolve(const CSSValue&);
    static bool interpolationRequiresStyleResolve(const CSSPrimitiveValue&);
    static bool interpolationRequiresStyleResolve(const CSSImageValue&);
    static bool interpolationRequiresStyleResolve(const CSSShadowValue&);
    static bool interpolationRequiresStyleResolve(const CSSValueList&);
    static bool interpolationRequiresStyleResolve(const CSSBasicShape&);

private:
    DeferredLegacyStyleInterpolation(PassRefPtr<CSSValue> start, PassRefPtr<CSSValue> end, CSSPropertyID id)
        : StyleInterpolation(InterpolableNumber::create(0), InterpolableNumber::create(1), id)
        , m_startCSSValue(start)
        , m_endCSSValue(end)
    {
    }

    RefPtr<CSSValue> m_startCSSValue;
    RefPtr<CSSValue> m_endCSSValue;
};

}

#endif
