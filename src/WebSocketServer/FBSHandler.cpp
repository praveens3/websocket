//// FBSHandler.cpp
//#include "FBSHandler.h"
//#include <iostream>
//#include <stdexcept>
//
//std::vector<uint8_t> FBSHandler::PackMyData(int id, const std::string& message) {
//    flatbuffers::FlatBufferBuilder builder;
//
//    auto messageOffset = builder.CreateString(message);
//    Example::MyDataBuilder myDataBuilder(builder);
//    myDataBuilder.add_id(id);
//    myDataBuilder.add_message(messageOffset);
//    auto myData = myDataBuilder.Finish();
//
//    builder.Finish(myData);
//    return std::vector<uint8_t>(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize());
//}
//
//std::vector<uint8_t> FBSHandler::PackMyData2(float value, const std::string& info) {
//    flatbuffers::FlatBufferBuilder builder;
//
//    auto infoOffset = builder.CreateString(info);
//    Example::MyData2Builder myData2Builder(builder);
//    myData2Builder.add_value(value);
//    myData2Builder.add_info(infoOffset);
//    auto myData2 = myData2Builder.Finish();
//
//    builder.Finish(myData2);
//    return std::vector<uint8_t>(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize());
//}
//
//void FBSHandler::UnpackData(const uint8_t* buf, size_t size) {
//    if (buf == nullptr || size == 0) {
//        throw std::invalid_argument("Buffer is null or empty");
//    }
//
//    // Assuming the first byte indicates the message type
//    uint8_t msgType = buf[0]; // Placeholder for actual message type
//
//    switch (msgType) {
//    case 0: // MyData type
//        UnpackMyData(buf + 1); // Skip the type byte
//        break;
//    case 1: // MyData2 type
//        UnpackMyData2(buf + 1); // Skip the type byte
//        break;
//    default:
//        std::cerr << "Unknown message type" << std::endl;
//        break;
//    }
//}
//
//void FBSHandler::UnpackMyData(const uint8_t* buf) {
//    auto myData = Example::GetMyData(buf);
//    PrintMyData(myData);
//}
//
//void FBSHandler::UnpackMyData2(const uint8_t* buf) {
//    auto myData2 = Example::GetMyData2(buf);
//    PrintMyData2(myData2);
//}
//
//void FBSHandler::PrintMyData(const Example::MyData* myData) {
//    if (myData) {
//        std::cout << "MyData - ID: " << myData->id() << ", Message: " << myData->message()->c_str() << std::endl;
//    }
//    else {
//        std::cerr << "Error: MyData is null" << std::endl;
//    }
//}
//
//void FBSHandler::PrintMyData2(const Example::MyData2* myData2) {
//    if (myData2) {
//        std::cout << "MyData2 - Value: " << myData2->value() << ", Info: " << myData2->info()->c_str() << std::endl;
//    }
//    else {
//        std::cerr << "Error: MyData2 is null" << std::endl;
//    }
//}
