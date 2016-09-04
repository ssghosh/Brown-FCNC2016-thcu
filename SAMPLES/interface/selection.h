/*
 * selection.h
 *
 *  Created on: 24 Aug 2016
 *      Author: jkiesele
 */

#ifndef selection_H_
#define selection_H_

#include "interface/basicAnalyzer.h"
#include "classes/DelphesClasses.h"


class selection: public d_ana::basicAnalyzer{
public:
	selection():d_ana::basicAnalyzer(){}
	~selection(){}


private:
	void analyze(size_t id);


};





#endif /* selection_H_ */
