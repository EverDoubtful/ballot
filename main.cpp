#include <iostream>
#include <fstream>
#include <cstring>

#include <algorithm>
#include <functional>
#include <array>
#include "pugixml.hpp"
using namespace std;

int main()
{
    ofstream file_log;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("ballots.xml");
    pugi::xml_node parent = doc.child("ballots");

    unsigned int arSize = 0;
    for (pugi::xml_node node = parent.first_child(); node; node = node.next_sibling())
      {
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
            arSize++;  //finding array size
      }

    unsigned char aBallots[arSize];
    unsigned int m = 0;
    for (pugi::xml_node node = parent.first_child(); node; node = node.next_sibling())
      {
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
            aBallots[m] = atoi(child.value());   //load elements from file
        m++;
      }

    string aCandidates[] = {
        "������",   //0
        "�����",
        "��������",  //2
        "�������",
        "�������",  //4
        "�����",
        "�����",    //6
        "��������",
        "�������",  //8
        "��������",
        "��������", //10
        "������",  //11
        "�������",  //12
        "�������",
        "�������",  //14
        "�����",
        "���������",  //16
        "�����",
        "�����",     //18
        "�����",
        "�������",   //20
        "���������",
        "�������",   //22
        "���������"
        };

    const unsigned int aCandSize = sizeof aCandidates / sizeof aCandidates[0];
    unsigned int aBallotsSize = sizeof aBallots / sizeof aBallots[0];

    unsigned int aWeights[aCandSize][2], aWRaw[aCandSize][2];  //0 - index, 1 - ballots
    unsigned int aWeightsSize = aCandSize; //sizeof aWeights / sizeof aWeights[0];
    file_log.open("ballots_res.log");
    unsigned int i = 0,k;
    while(i < aWeightsSize)
    {
        aWeights[i][0] = i;
        aWeights[i][1] = 0;
        i++;
    }

    i = 0;
    unsigned int first_votes = 0, second_votes = 0, third_votes = 0;
    signed int first_index = -1, second_index = -1, third_index = -1;
    unsigned int element;

    while(i < aBallotsSize)
    {
      //got new info
      aWeights[aBallots[i]][1]++;

      //copy
      k = 0;
      while(k < aWeightsSize)
      {
       aWRaw[k][0] = aWeights[k][0];
       aWRaw[k][1] = aWeights[k][1];
       k++;
      }
      //sorting by indexes
      qsort(aWRaw, aWeightsSize, sizeof(*aWRaw),
        [](const void *arg1, const void *arg2)->int
        {
            int const *lhs = static_cast<int const*>(arg1);
            int const *rhs = static_cast<int const*>(arg2);
            return (lhs[1] > rhs[1] ? -1
                :  ((rhs[1] > lhs[1] ? 1 : 0)));
        });

       first_index  = aWRaw[0][0];
       second_index = aWRaw[1][0];
       third_index  = aWRaw[2][0];

       first_votes  = aWRaw[0][1];
       second_votes = aWRaw[1][1];
       third_votes  = aWRaw[2][1];

      //output _index
      if(third_votes)
        file_log << "���������� " << i+1 << " ���. (1) �������� N " << first_index+1 << " - " << aCandidates[first_index] << "[" << first_votes << " ���.];"\
               << "(2) �������� N " << second_index+1 << " - " << aCandidates[second_index] << "[" << second_votes << " ���.];"\
               << "(3) �������� N " << third_index+1 << " - " << aCandidates[third_index] << "[" << third_votes << " ���.]." <<  "\n";
      else if(second_votes)
        file_log << "���������� " << i+1 << " ���. (1) �������� N " << first_index+1 << " - " << aCandidates[first_index] << "[" << first_votes << " ���.];"\
               << "(2) �������� N " << second_index+1 << " - " << aCandidates[second_index] << "[" << second_votes << " ���.]\n";
      else
        file_log << "���������� " << i+1 << " ���. (1) �������� N " << first_index+1 << " - " << aCandidates[first_index] << "[" << first_votes << " ���.]\n";

      i++;
    }

    file_log.close();
    return 0;
}
