/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// This is a modification of greeter_client.cc from grpc examples.
// Comments have been removed to make it easier to follow the code.
// For comments please refer to the original example.

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "hellostreamingworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;
using hellostreamingworld::HelloRequest;
using hellostreamingworld::HelloReply;
using hellostreamingworld::MultiGreeter;

class GreeterClient
{
public:
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(MultiGreeter::NewStub(channel))
    {}

    void SayHello(const std::string& user, const std::string& num_greetings)
    {
        HelloRequest request;
        request.set_name(user);
        request.set_num_greetings(num_greetings);

        ClientContext context;
        std::unique_ptr<ClientReader<HelloReply>> reader(stub_->sayHello(&context, request));

        HelloReply reply;
        while (reader->Read(&reply)) 
        {
            std::cout << "Got reply: " << reply.message() << std::endl;
        }

        Status status = reader->Finish();
        if (status.ok()) 
        {
            std::cout << "sayHello rpc succeeded." << std::endl;
        } 
        else 
        {
            std::cout << "sayHello rpc failed." << std::endl;
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
        }
    }

private:
    std::unique_ptr<MultiGreeter::Stub> stub_;
};

int main(int argc, char** argv)
{
    GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user("world");
    greeter.SayHello(user, "123");

    return 0;
}
