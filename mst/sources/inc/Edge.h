//
// Created by carlos on 06/03/19.
//

#ifndef MRP_ARC_H
#define MRP_ARC_H


class Edge {

private:
    int o, d;

public:
    Edge(int o, int d);

    int getO() const;

    int getD() const;
};


#endif //MRP_ARC_H
