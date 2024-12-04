cmake_minimum_required(VERSION 3.27)
project(Praktikum_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(Praktikum_3 main.c
        queue.c
        queue.h
        consumer.c
        consumer.h
        producer.c
        producer.h
        observer.c
        observer.h
        p3errno.h
)
