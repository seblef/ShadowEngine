
#ifndef _CHARACTERDB_H_
#define _CHARACTERDB_H_

#include "Character.h"

class CharacterDB : public TDataBase<Character>
{
public:

	CharacterDB(const string& playersFile);

	void				unloadCharacters();
};

#endif