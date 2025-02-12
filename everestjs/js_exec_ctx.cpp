// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2021 Pionix GmbH and Contributors to EVerest
#include "js_exec_ctx.hpp"
#include "utils.hpp"

void JsExecCtx::tramp(Napi::Env env, Napi::Function callback, std::nullptr_t* context, JsExecCtx* this_) {
    (void) context; // this is unused in this callback

    try {
        std::vector<napi_value> args{this_->result_handler_ref.Value()};
        if (this_->arg_func != nullptr) {
            // append args from our arg function via move magic ...
            auto append_args = this_->arg_func(env);
            args.reserve(args.size() + append_args.size());
            std::move(std::begin(append_args), std::end(append_args), std::back_inserter(args));
            append_args.clear();
        }

        callback.Call(args);
    } catch (std::exception& e) {
        EVLOG_AND_RETHROW(env);
    }
}

Napi::Value JsExecCtx::on_fulfill(const Napi::CallbackInfo& info) {
    JsExecCtx* this_ = reinterpret_cast<JsExecCtx*>(info.Data());
    if (this_->res_func != nullptr) {
        this_->res_func(info, false);
    }
    this_->promise.set_value();
    return info.Env().Undefined();
}

Napi::Value JsExecCtx::on_reject(const Napi::CallbackInfo& info) {
    JsExecCtx* this_ = reinterpret_cast<JsExecCtx*>(info.Data());
    if (this_->res_func != nullptr) {
        this_->res_func(info, true);
    } else {
        // there is no catch handler registered, so we throw
        throw Napi::Error::New(
            info.Env(),
            "JsExecCtx call into javascript code got rejected and could not be handled (rejection handler not defined");
    }

    this_->promise.set_value();
    return info.Env().Undefined();
}

void JsExecCtx::exec(const ArgFuncType& arg_func, const ResFuncType& res_func) {
    // FIXME (aw): we're blocking all other threads trying to call this function
    //             a proper solution should be found
    std::unique_lock<std::mutex> lock(exec_mutex);
    this->arg_func = arg_func;
    this->res_func = res_func;

    promise = std::promise<void>();

    tsfn.BlockingCall(this);
    promise.get_future().get();
}
