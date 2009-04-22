/*
  This file is part of the Astrometry.net suite.
  Copyright 2006, 2007 Dustin Lang, Keir Mierle and Sam Roweis.

  The Astrometry.net suite is free software; you can redistribute
  it and/or modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation, version 2.

  The Astrometry.net suite is distributed in the hope that it will be
  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the Astrometry.net suite ; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

#ifndef VERIFY_H
#define VERIFY_H

#include "kdtree.h"
#include "matchobj.h"
#include "bl.h"
#include "starkd.h"
#include "sip.h"
#include "bl.h"
#include "starxy.h"
#include "index.h"

struct verify_field_t {
    const starxy_t* field;
    double* fieldcopy;
    kdtree_t* ftree;
};
typedef struct verify_field_t verify_field_t;

/*
  Uses the following entries in the "mo" struct:
  -wcs_valid
  -wcstan
  -center
  -radius
  -field[]
  -star[]

  Sets the following:
  -nfield
  -noverlap
  -nconflict
  -nindex
  -(matchobj_compute_derived() values)
  -logodds
  -corr_field
  -corr_index
 */
void verify_hit(index_t* index,
                MatchObj* mo,
                sip_t* sip, // if non-NULL, verify this SIP WCS.
                verify_field_t* vf,
                double verify_pix2,
                double distractors,
                double fieldW,
                double fieldH,
                double logratio_tobail,
                bool distance_from_quad_bonus,
				int dimquads,
                bool fake_match,
				double logodds_tokeep);

double verify_star_lists(const double* refxys, int NR,
						 const double* testxys, const double* testsigma2s, int NT,
						 double effective_area,
						 double distractors,
						 double logodds_bail,
						 double logodds_accept,
						 int* p_besti,
						 double** p_all_logodds, int** p_theta,
						 double* p_worstlogodds);

verify_field_t* verify_field_preprocess(const starxy_t* fieldxy);

void verify_field_free(verify_field_t* vf);

void verify_get_index_stars(const double* fieldcenter, double fieldr2,
							const startree_t* skdt, const sip_t* sip, const tan_t* tan,
							double fieldW, double fieldH,
							double** p_indexradec,
							double** p_indexpix, int** p_starids, int* p_nindex);


double* verify_compute_sigma2s(const verify_field_t* vf, const MatchObj* mo,
							   double verify_pix2, bool do_gamma);

double* verify_compute_sigma2s_arr(const double* xy, int NF,
								   const double* qc, double Q2,
								   double verify_pix2, bool do_gamma);

void verify_get_uniformize_scale(int cutnside, double scale, int W, int H, int* cutnw, int* cutnh);

/**

 -if p_bincenters in non-NULL, it gets the x,y positions of the nw*nh
  bin centers

 -if p_bincounts is non-NULL, it gets the number of stars in each of
  the nw*nh bins.

 -if p_binids is non-NULL, it gets the bin index of each star.  The
 array is in the same order as the return value.  The bin index can be
 used to reference the "p_bincounts" or "p_bincenters" arrays.

 -returns: an array of indices into the original field array, in the
  uniformized order.

 */
int* verify_uniformize_field(verify_field_t* vf,
							 double fieldW, double fieldH,
							 int nw, int nh,
							 int** p_bincounts,
							 double** p_bincenters,
							 int** p_binids);



void verify_get_all_matches(const double* refxys, int NR,
							const double* testxys, const double* testsigma2s, int NT,
							double effective_area,
							double distractors,
							double nsigma,
							double limit,
							il*** p_reflist,
							dl*** p_problist);

#endif
