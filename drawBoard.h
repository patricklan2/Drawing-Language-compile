#pragma once

#ifndef DWARBOARD_H
#define DWARBOARD_H

#include"scanner.h"
#include"expression.h"
#include"parser.h"

class DrawBoard {
    int sleepTime = 3000;
    int length = 720;
    int weight = 640;
public:
	DrawBoard() {};

	void draw(const char* filename) {
        try
        {
            initgraph(length,weight);
            Parser parser = Parser(filename);
            printf("start draw\n");
            parser.compile();
            Sleep(sleepTime);
            closegraph();
        }
        catch (Error& e) {
            closegraph();
            e.printOut();
        }
	}

    void setSize(int height, int weight) {
        this->length = height;
        this->weight = weight;
    }

    void setSleepTime(int sleepTime) {
        this->sleepTime = sleepTime;
    }
};

#endif
