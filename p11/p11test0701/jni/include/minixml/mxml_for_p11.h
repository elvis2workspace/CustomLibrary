#ifndef MXML_FOR_P11_H_
#define MXML_FOR_P11_H_

#ifdef __cplusplus
extern "C" {
#endif

int InputPin(int argc, char *argv[]);

CK_RV ReadXml(unsigned int* chooseitem, unsigned int* itemdatalen, unsigned int* itemlooptime, unsigned int* itemcount);
CK_RV WriteXml(int CNT);

#ifdef __cplusplus
}
#endif

#endif