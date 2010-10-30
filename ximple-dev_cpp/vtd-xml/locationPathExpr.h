/* 
 * Copyright (C) 2002-2010 XimpleWare, info@ximpleware.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#pragma once
#ifndef LOCATION_PATH_EXPR_H
#define LOCATION_PATH_EXPR_H
#include "expr.h"
#include "intHash.h"
#include "textIter.h"

namespace com_ximpleware{

	class LocationPathExpr: Expr{
	friend class xpathParser;
	public:
		LocationPathExpr();
		virtual ~LocationPathExpr();
		bool evalBoolean(VTDNav *vn);
		double evalNumber(VTDNav *vn);
		int evalNodeSet(VTDNav *vn);
		UCSChar* evalString(VTDNav *vn);
		void setStep(Step* st){
			s = st;
		}
		void reset(VTDNav *vn);
		void toString(UCSChar *s);

		bool isNumerical();
		bool isNodeSet();
		bool isString();
		bool isBoolean();

		bool requireContextSize();
		void setContextSize(int size);

		void setPosition(int pos);
		int adjust(int n);
		pt pathType;

	private:
		bool isUnique_lpe(int i);
		int computeContextSize(Predicate *p, VTDNav *vn);
		int process_ancestor_or_self(VTDNav *vn);
		int process_ancestor(VTDNav *vn);
		int process_attribute(VTDNav *vn);
		int process_child(VTDNav *vn);
		int process_DDFP(VTDNav *vn);
		int process_following_sibling(VTDNav *vn);
		int process_parent(VTDNav *vn);
		int process_preceding_sibling(VTDNav *vn);
		int process_self(VTDNav *vn);
		int process_namespace(VTDNav *vn);
		void selectNodeType(TextIter *ti);
		bool isUnique(int i){
			return ih->isUnique(i);
		}
		Step* s;
		Step* currentStep;
		LPstate state;
		/*FastIntBuffer* fib; // for uniqueness checking */
		IntHash *ih; /* for uniqueness checking*/

	};

	UCSChar *axisName(axisType i);

}

#endif
