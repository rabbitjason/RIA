#ifndef _I_EVAL_SINK_H_
#define _I_EVAL_SINK_H_

class IEvalSink
{
public:
    virtual void OnEvalReply(const char *id, int type, const void *message, int size) = 0;
};
#endif
