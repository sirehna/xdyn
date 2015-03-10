/*
 * TestMeshes.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: cady
 */

#include "TestMeshes.hpp"

TestMesh case_0()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,16);
    ret.all_nodes << 0, 3, 6, 5, 6, 3, 0, 1,   3, 4, 4.5, 4,   3, 2, 1.5, 2,
                     0, 1, 0, 3, 6, 5, 6, 3, 1.5, 2,   3, 4, 4.5, 4,   3, 2,
                     0, 0, 0, 0, 0, 0, 0, 0,   0, 0,   0, 0,   0, 0,   0, 0;
    ret.edges = {{13,15},{15,9},{9,11},{11,13}};
    return ret;
}

TestMesh case_1()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,5);
    ret.all_nodes << 0, 1, 1, 0.5, 0,
                     0, 0, 1, 0.5, 1,
                     0, 0, 0,   0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{2,4},{4,0},{3,4}};
    return ret;
}

TestMesh case_2()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,5);
    ret.all_nodes << 0, 1, 1, 0.5, 0,
                     0, 0, 1, 1.5, 1,
                     0, 0, 0,   0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{2,4},{4,0},{3,4}};
    return ret;
}

TestMesh case_3()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,8);
    ret.all_nodes << 0, 3, 3, 2, 2, 1, 1, 0,
                     0, 0, 2, 2, 1, 1, 2, 2,
                     0, 0, 0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,0}};
    return ret;
}

TestMesh case_4()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes << 0, 3, 2, 3, 1, 0,
                     0, 0, 1, 2, 1, 2,
                     0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{2,4},{3,5},{1,3},{4,5},{5,0},{4,0}};
    return ret;
}

TestMesh case_5()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes << 0, 1, 2, 2, 1, 0,
                     0, 0, 0, 1, 1, 1,
                     0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{1,5},{5,0},{1,4}};
    return ret;
}

TestMesh case_6()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes << 0, 1, 2, 2, 1, 0,
                     0, 0, 0, 2, 1, 2,
                     0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,0},{1,4}};
    return ret;
}

TestMesh case_7()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,8);
    ret.all_nodes << 0, 1, 2, 2, 1, 0, 1, 1,
                     0, 1, 0, 5, 4, 5, 2, 3,
                     0, 0, 0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,0},{0,6},{6,2},{5,7},{7,3}};
    return ret;
}

TestMesh case_8()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes << 0, 1, 1, 0.5, 0, 0.5,
                     0, 0, 1,   2, 1, 0.5,
                     0, 0, 0,   0, 0,   0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,0},{3,5}};
    return ret;
}

TestMesh case_9()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,5);
    ret.all_nodes << 0, 1, 2, 2, 0,
                     0, 1, 0, 2, 2,
                     0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,1},{1,4},{4,0}};
    return ret;
}

TestMesh case_10()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,16);
    ret.all_nodes << 0, 3, 6, 5, 6, 3, 0, 1,   3, 4, 4.5, 4,   3, 2, 1.5, 2,
                     0, 1, 0, 3, 6, 5, 6, 3, 1.5, 2,   3, 4, 4.5, 4,   3, 2,
                     0, 0, 0, 0, 0, 0, 0, 0,   0, 0,   0, 0,   0, 0,   0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,0},{0,8},{8,2},{2,10},{10,4},{4,12},{12,6},{6,14},{14,0},{8,9},{9,10},{10,11},{11,12},{12,13},{13,14},{14,15},{15,8},{1,8},{3,10},{5,12},{7,14}};
    return ret;
}

TestMesh case_11()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,8);
    ret.all_nodes << 2, 3, 4, 5, 4, 3, 2, 1,
                     0, 1, 2, 3, 6, 5, 4, 3,
                     0, 0, 0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{2,3},{3,4},{4,5},{5,6},{6,7},{7,0},{5,2},{1,6}};
    return ret;
}

TestMesh case_12()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,4);
    ret.all_nodes << -1, 0,  0, -1,
                      1, 0, -1, -1,
                      0, 0,  0,  0;
    ret.edges = {{0,1},{1,2},{1,3}};
    return ret;
}

TestMesh case_13()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,4);
    ret.all_nodes <<  0, 1, 1, 0,
                      0, 0, 1, 1,
                      0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{1,3}};
    return ret;
}

TestMesh case_14()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes <<  0, 1, 1, 2, 2, 3,
                      0, 0, 1, 0, 1, 1,
                      0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,0},{3,4},{4,5},{5,3}};
    return ret;
}

TestMesh case_15()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,4);
    ret.all_nodes <<  0, 0, 1, 1,
                      1, 0, 0, 1,
                      0, 0, 0, 0;
    ret.edges = {{0,1},{2,1},{2,3},{3,0},{1,3}};
    return ret;
}

TestMesh case_16()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,8);
    ret.all_nodes <<  0, 0, 1, 1, 0, 0, 1, 1,
                      1, 0, 0, 1, 3, 2, 2, 3,
                      0, 0, 0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{2,1},{2,3},{3,0},{1,3},{4,5},{5,6},{6,7},{7,4},{5,7}};
    return ret;
}

TestMesh case_17()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,8);
    ret.all_nodes << -2, -3, -4, -5, -4, -3, -2, -1,
                      0,  1,  2,  3,  6,  5,  4,  3,
                      0,  0,  0,  0,  0,  0,  0,  0;
    ret.edges = {{0,1},{2,3},{3,4},{4,5},{5,6},{6,7},{7,0},{5,2},{1,6}};
    return ret;
}

TestMesh case_18()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,4);
    ret.all_nodes <<  0, 0, 1, 1,
                      1, 0, 0, 1,
                      0, 0, 0, 0;
    ret.edges = {{0,1},{2,1},{3,2},{0,3},{1,3}};
    return ret;
}

TestMesh case_19()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,4);
    ret.all_nodes <<  0, 0, 1, 1,
                      1, 0, 0, 1,
                      0, 0, 0, 0;
    ret.edges = {{0,1},{2,3},{1,2},{1,3},{0,3}};
    return ret;
}

TestMesh case_20()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes <<  0, 0, 0, 1, 1, 1,
                      0, 1, 2, 0, 1, 2,
                      0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{4,3},{5,4},{2,5},{3,0},{3,1},{4,1},{4,2}};
    return ret;
}

TestMesh case_21()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes <<  0, 1, 1, 1, 0, 0,
                      0, 0, 1, 2, 2, 1,
                      0, 0, 0, 0, 0, 0;
    ret.edges = {{5,0},{4,5},{2,1},{2,3},{3,4},{5,2},{5,3},{0,1}};
    return ret;
}

TestMesh case_22()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes <<  0, 0, 1, 2, 2, 1,
                      0, 1, 1, 1, 0, 0,
                      0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,0}};
    return ret;
}
