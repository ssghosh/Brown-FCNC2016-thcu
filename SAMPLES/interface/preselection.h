/*
 * preselection.h
 *
 *  Created on: 24 Aug 2016
 *      Author: jkiesele
 */

#ifndef preselection_H_
#define preselection_H_

#include "interface/basicAnalyzer.h"
#include "classes/DelphesClasses.h"


class preselection: public d_ana::basicAnalyzer{
public:
	preselection():d_ana::basicAnalyzer(){}
	~preselection(){}


private:
	void analyze(size_t id);


};





#endif /* preselection_H_ */
