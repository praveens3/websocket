//// FBSHandler.h
//#ifndef FBSHANDLER_H
//#define FBSHANDLER_H
//
//#include <cstdint>
//#include <string>
//#include <vector>
//#include "MyData.h"   // Include your generated FlatBuffers header for MyData
//#include "MyData2.h"  // Include your generated FlatBuffers header for MyData2
//
//enum class MessageType {
//    MyData,
//    MyData2
//};
//
//class FBSHandler {
//public:
//    // Packs data into a FlatBuffer for specific message types
//    static std::vector<uint8_t> PackMyData(int id, const std::string& message);
//    static std::vector<uint8_t> PackMyData2(float value, const std::string& info);
//
//    // Unpacks data from a FlatBuffer
//    static void UnpackData(const uint8_t* buf, size_t size);
//
//private:
//    // Specific unpack handlers
//    static void UnpackMyData(const uint8_t* buf);
//    static void UnpackMyData2(const uint8_t* buf);
//
//    // Utility functions to print data
//    static void PrintMyData(const Example::MyData* myData);
//    static void PrintMyData2(const Example::MyData2* myData2);
//};
//
//#endif // FBSHANDLER_H
