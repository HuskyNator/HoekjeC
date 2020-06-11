#ifndef VERVER_H
#define VERVER_H

#define VERVER_GROOTTE 1024

typedef struct Verver Verver;

struct Verver
{
    unsigned int verfProgramma;
};

Verver *maakVerver(const char *hoekVerfLocatie, const char *scherfVerfLocatie);
void verwijderVerver(Verver *verver);

void gebruikVerver(Verver *verver);
void zetVerverInt(Verver *verver, const char *naam, int waarde);
void zetVerverFloat(Verver *verver, const char *naam, float waarde);
void zetVerverFloat4v(Verver *verver, const char *naam, float waarden[]);
// En Meer

#endif