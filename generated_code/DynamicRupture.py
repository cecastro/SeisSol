#!/usr/bin/env python3
##
# @file
# This file is part of SeisSol.
#
# @author Carsten Uphoff (c.uphoff AT tum.de, http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
#
# @section LICENSE
# Copyright (c) 2016-2018, SeisSol Group
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# @section DESCRIPTION
#

from yateto import Tensor, Scalar, simpleParameterSpace
from yateto.input import parseJSONMatrixFile
from multSim import OptionalDimTensor
from common import generate_kernename_prefix

def addKernels(generator, aderdg, matricesDir, dynamicRuptureMethod, platforms):
  if dynamicRuptureMethod == 'quadrature':
    numberOfPoints = (aderdg.order+1)**2
  elif dynamicRuptureMethod == 'cellaverage':
    numberOfPoints = int(4**math.ceil(math.log(aderdg.order*(aderdg.order+1)/2,4)))
  else:
    raise ValueError('Unknown dynamic rupture method.')

  clones = dict()

  # Load matrices
  db = parseJSONMatrixFile('{}/dr_{}_matrices_{}.json'.format(matricesDir, dynamicRuptureMethod, aderdg.order), clones, alignStride=aderdg.alignStride, transpose=aderdg.transpose)

  # Determine matrices
  # Note: This does only work because the flux does not depend on the mechanisms in the case of viscoelastic attenuation
  godunov_spp = aderdg.godunov_spp()
  godunovMatrix = Tensor('godunovMatrix', godunov_spp.shape, spp=godunov_spp)
  flux_solver_spp = aderdg.flux_solver_spp()
  fluxSolver    = Tensor('fluxSolver', flux_solver_spp.shape, spp=flux_solver_spp)
  
  gShape = (numberOfPoints, aderdg.numberOfQuantities())
  godunovState = OptionalDimTensor('godunovState', aderdg.Q.optName(), aderdg.Q.optSize(), aderdg.Q.optPos(), gShape, alignStride=True)

  generator.add('rotateGodunovStateLocal',
                godunovMatrix['qp'] <= aderdg.Tinv['kq'] * aderdg.QgodLocal['kp'],
                platform='cpu')
  generator.add('rotateGodunovStateNeighbor',
                godunovMatrix['qp'] <= aderdg.Tinv['kq'] * aderdg.QgodNeighbor['kp'],
                platform='cpu')

  fluxScale = Scalar('fluxScale')
  generator.add('rotateFluxMatrix',
                fluxSolver['qp'] <= fluxScale * aderdg.starMatrix(0)['qk'] * aderdg.T['pk'],
                platform='cpu')

  def godunovStateGenerator(i,h):
    target = godunovState['kp']
    term = db.V3mTo2n[i,h][aderdg.t('kl')] * aderdg.Q['lq'] * godunovMatrix['qp']
    if h == 0:
      return target <= term
    return target <= target + term
  godunovStatePrefetch = lambda i,h: godunovState
  generator.addFamily('godunovState',
                      simpleParameterSpace(4,4),
                      godunovStateGenerator,
                      godunovStatePrefetch,
                      platform='cpu')

  nodalFluxGenerator = lambda i,h: aderdg.extendedQTensor()['kp'] <= aderdg.extendedQTensor()['kp'] + db.V3mTo2nTWDivM[i,h][aderdg.t('kl')] * godunovState['lq'] * fluxSolver['qp']
  nodalFluxPrefetch = lambda i,h: aderdg.I

  for platform in platforms:
    name_prefix = generate_kernename_prefix(platform)
    generator.addFamily(f'{name_prefix}nodalFlux',
                        simpleParameterSpace(4,4),
                        nodalFluxGenerator,
                        nodalFluxPrefetch,
                        platform=platform)
