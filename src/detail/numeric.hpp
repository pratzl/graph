#ifndef __BGL17_NUMERIC_HPP
#define __BGL17_NUMERIC_HPP

namespace std {

template <class _InputIterator, class _OutputIterator, class _Tp, class _BinaryOp>
inline
_OutputIterator
exclusive_scan(_InputIterator __first, _InputIterator __last,
               _OutputIterator __result, _Tp __init, _BinaryOp __b)
{
    if (__first != __last)
    {
        _Tp __saved = __init;
        do
        {
            __init = __b(__init, *__first);
            *__result = __saved;
            __saved = __init;
            ++__result;
        } while (++__first != __last);
    }
    return __result;
}

template <class _InputIterator, class _OutputIterator, class _Tp>
inline
_OutputIterator
exclusive_scan(_InputIterator __first, _InputIterator __last,
               _OutputIterator __result, _Tp __init)
{
    return exclusive_scan(__first, __last, __result, __init, plus<>());
}

} // namespace

#endif