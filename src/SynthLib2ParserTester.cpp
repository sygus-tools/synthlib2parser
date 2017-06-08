//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
//
// Copyright (c) 2017,
// Ammar Ben Khadra,
// University of Kaiserslautern
//
// Copyright (c) 2013,
// Abhishek Udupa, Mukund Raghothaman,
// The University of Pennsylvania
//
//===----------------------------------------------------------------------===//
//

#include <SynthLib2ParserIFace.hpp>

using namespace SynthLib2Parser;

int main(int argc, char* argv[])
{
    ::SynthLib2Parser::SynthLib2Parser Parser;
    try {
        Parser(argv[1]);
    } catch (const exception& Ex) {
        cout << Ex.what() << endl;
        exit(1);
    }
    cout << *(Parser.GetProgram()) << endl;
}
