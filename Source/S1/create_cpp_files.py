# -*- coding: utf-8 -*-
import sys

def create_cpp_files(class_name, base_class_name, module_name):
    # 부모 클래스의 첫 문자에 따라 접두어 설정
    first_char = base_class_name[0]
    if first_char == 'A':
        uclass_name = "A" + class_name  # 'A' 접두어 붙이기
        cclass_name = "A" + class_name  # 'A' 접두어 붙이기
    elif first_char == 'U':
        uclass_name = "U" + class_name  # 'B' 접두어 붙이기
        cclass_name = "U" + class_name  # 'B' 접두어 붙이기
    else:
        uclass_name = class_name  # 기본 이름
        cclass_name = class_name  # 기본 이름

    if base_class_name == "AActor":
        include_base_name = "GameFramework/Actor" # Actor Header 추가
    else:
        include_base_name = base_class_name


    # 파일 이름 생성
    header_filename = "{}.h".format(class_name)
    cpp_filename = "{}.cpp".format(class_name)

    # 헤더 파일 내용
    header_content = """
// Copyright (c) S1

#pragma once

#include "{}.h"
#include "{}.generated.h"

UCLASS()
class {} {} : public {}
{{
    GENERATED_BODY()

public:
    {}(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


}}; 
""".format(include_base_name, class_name, module_name, uclass_name, base_class_name, cclass_name)

    # cpp 파일 내용
    cpp_content = """
// Copyright (c) S1

#include "{}.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME({})

{}::{}(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{{

}}


""".format(class_name, class_name, cclass_name, cclass_name, cclass_name, class_name)

    # 파일 생성 및 저장
    with open(header_filename, 'w') as header_file:
        header_file.write(header_content)

    with open(cpp_filename, 'w') as cpp_file:
        cpp_file.write(cpp_content)

    print("Files created: {}, {}".format(header_filename, cpp_filename))

# 명령어 인자로 클래스 이름, 베이스 클래스 이름 및 모듈 이름을 받아 처리
if len(sys.argv) == 4:
    class_name = sys.argv[1]
    base_class_name = sys.argv[2]
    module_name = sys.argv[3]
    create_cpp_files(class_name, base_class_name, module_name)
else:
    print("Error: Incorrect number of arguments. Usage: python create_cpp_files.py <class_name> <base_class_name> <module_name>")
