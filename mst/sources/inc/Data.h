//
// Created by carlos on 05/03/19.
//

#ifndef MRP_DATA_H
#define MRP_DATA_H

#include "Include.h"
#include "Edge.h"


class Data {

public:
    vector<Edge *> edges;
    int n, m;

    Data(const char *instance);

    void showData();
};


#endif //MRP_DATA_H
