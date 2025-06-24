# mfemFieldTesting
testing mfem field operations

SCOREC RHEL9 environment setup

```
module use /opt/scorec/spack/rhel9/v0201_4/lmod/linux-rhel9-x86_64/Core/
module load \
gcc/12.3.0-iil3lno \
exuberant-ctags/5.8-bhzm3ct \
cmake/3.26.3-2duxfcd \
mpich/4.1.1-xpoyz4t \
gdb/13.1-z4rrapk \
cuda/12.1.1-zxa4msk \
hypre/2.28.0-dljbagf \
metis/5.1.0-65szzoy
```

mfem (tested with master @ e9782cf437 on checkers) cmake config command 

```
hypre=$HYPRE_ROOT
unset HYPRE_ROOT
metis=$METIS_ROOT
unset METIS_ROOT
cmake -B buildMfem -S mfem \
-DCMAKE_BUILD_TYPE=Debug \
-DMFEM_USE_MPI=YES \
-DMFEM_USE_METIS_5=YES \
-DMFEM_DEBUG=NO \
-DMFEM_ENABLE_EXAMPLES=NO \
-DMFEM_ENABLE_MINIAPPS=NO \
-DMFEM_USE_PUMI=NO \
-DHYPRE_DIR=$hypre \
-DMETIS_DIR=$metis \
-DMFEM_USE_CUDA=YES \
-DCUDA_ARCH=sm_86 \
-DCMAKE_INSTALL_PREFIX=$PWD/install
```

cmake config command for this repo

```
cmake -S . -B buildFieldEval -DCMAKE_CUDA_ARCHITECTURES=86
```

