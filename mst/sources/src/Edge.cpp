//
// Created by carlos on 06/03/19.
//

#include "../headers/Edge.h"

Edge::Edge(int o, int d) {
    this->o = o;
    this->d = d;
}

int Edge::getO() const {
    return o;
}

int Edge::getD() const {
    return d;
}
