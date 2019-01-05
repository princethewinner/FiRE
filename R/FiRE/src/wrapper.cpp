/*
 * Copyright (C) 2018 Aashi Jindal, Prashant Gupta, Jayadeva, Debarka Sengupta (aashi.jindal@ee.iitd.ac.in, prashant.gupta@ee.iitd.ac.in, jayadeva@ee.iitd.ac.in, debarka@iiitd.com). All Rights Reserved.
 *
 * This file is part of FiRE.
 *
 * FiRE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FiRE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FiRE.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "FiRE.h"

RCPP_MODULE(fire){
    using namespace Rcpp ;
    class_<FiRE>("FiRE")
    .constructor<int, int, int, int, int>()
    .constructor<int, int>()
    .property("ths", &FiRE::ths)
    .property("w", &FiRE::w)
    .property("d", &FiRE::d)
    .property("b", &FiRE::b)
    .method("fit", &FiRE::fit)
    .method("score", &FiRE::score);
}
