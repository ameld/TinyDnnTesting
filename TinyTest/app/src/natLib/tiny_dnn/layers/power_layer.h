/*
    Copyright (c) 2016, Taiga Nomi
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include "tiny_dnn/util/util.h"
#include "tiny_dnn/layers/layer.h"
#include <cmath>

namespace tiny_dnn {


/**
 * y = x^factor
 **/
class power_layer : public layer {
public:
    typedef layer Base;

    power_layer(const shape3d& in_shape, float_t factor)
        : layer({ vector_type::data }, { vector_type::data }),
        in_shape_(in_shape), factor_(factor) {
    }

    power_layer(const layer& prev_layer, float_t factor)
        : layer({ vector_type::data }, { vector_type::data }),
        in_shape_(prev_layer.out_shape()[0]), factor_(factor) {
    }

    std::string layer_type() const override {
        return "power";
    }

    std::vector<shape3d> in_shape() const override {
        return {in_shape_};
    }

    std::vector<shape3d> out_shape() const override {
        return {in_shape_};
    }

    void forward_propagation(const std::vector<tensor_t*>& in_data,
                             std::vector<tensor_t*>& out_data) override {
        const tensor_t& x = *in_data[0];
        tensor_t&       y = *out_data[0];

        for (cnn_size_t i = 0; i < x.size(); i++) {
            std::transform(x[i].begin(), x[i].end(), y[i].begin(), [=](float_t x) {
                return std::pow(x, factor_); 
            });
        }
    }

    void back_propagation(const std::vector<tensor_t*>& in_data,
                          const std::vector<tensor_t*>& out_data,
                          std::vector<tensor_t*>&       out_grad,
                          std::vector<tensor_t*>&       in_grad) override {
        tensor_t&       dx = *in_grad[0];
        const tensor_t& dy = *out_grad[0];
        const tensor_t& x  = *in_data[0];
        const tensor_t& y = *out_data[0];

        for (cnn_size_t i = 0; i < x.size(); i++) {
            for (cnn_size_t j = 0; j < x[i].size(); j++) {
                // f(x) = x^factor
                // ->
                //   dx = dy * df(x)
                //      = dy * factor * x^(factor-1)
                //      = dy * factor * f(x) / x
                //      = dy * factor * y / x
                if (std::abs(x[i][j]) > 1e-10) {
                    dx[i][j] = dy[i][j] * factor_ * y[i][j] / x[i][j];
                }
                else {
                    dx[i][j] = dy[i][j] * factor_ * std::pow(x[i][j], factor_ - 1.0f);
                }
            }
        }
    }

    template <class Archive>
    static void load_and_construct(Archive & ar, cereal::construct<power_layer> & construct) {
        shape3d in_shape;
        float_t factor;

        ar(cereal::make_nvp("in_size", in_shape), cereal::make_nvp("factor", factor));
        construct(in_shape, factor);
    }

    template <class Archive>
    void serialize(Archive & ar) {
        serialize_prolog(ar, this);
        ar(cereal::make_nvp("in_size", in_shape_), cereal::make_nvp("factor", factor_));
    }
private:

    shape3d in_shape_;
    float_t factor_;
};

} // namespace tiny_dnn

CNN_REGISTER_LAYER_SERIALIZER(tiny_dnn::power_layer, power);
