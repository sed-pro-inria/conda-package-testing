#include <stdio.h>

#include "metis.h"

// See the mesh here:
// https://github.com/dfroger/metis4py/blob/master/part_mesh_nodal.ipynb

// Number of elements in the mesh (18 triangles).
#define NE 18

// Number of nodes in the mesh (16 points).
#define NN 16

// All node have equal weight.
#define VWGT NULL

// All node have equal size.
#define VSIZE NULL

// All partition have equal weight.
#define TPWGTS NULL

int main()
{

    // =====================================
    // Set Metis inputs values.
    // =====================================

    // Number of elements in the mesh (18 triangles).
    idx_t ne = NE;

    // Number of nodes in the mesh (16 points).
    idx_t nn = NN;

    // Indices for eind.
    idx_t eptr[NE+1] = { 0, 3, 6, 9,
                        12,15,18,21,
                        24,27,30,33,
                        36,39,42,45,
                        48,51,54};

    // Triangles point indices.
    idx_t eind[NE*3] = {0, 4, 1,  1, 5, 2,  2, 6, 3,
                        4, 8, 5,  5, 9, 6,  6,10, 7,
                        8, 9,12,  9,10,13, 10,11,14,
                        1, 4, 5,  2, 5, 6,  3, 6, 7,
                        5, 8, 9,  6, 9,10,  7,10,11,
                        9,12,13, 10,13,14, 11,14,15};

    // Number of partition.
    idx_t nparts = 3;

    // Options.
    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    options[METIS_OPTION_SEED] = 0;

    // =====================================
    // Allocate Metis outputs.
    // =====================================

    // Total communication volume.
    idx_t objval;

    // Element and node partition vectors.
    idx_t epart[NE];
    idx_t npart[NN];

    // =====================================
    // Call Metis function.
    // =====================================

    int err = METIS_PartMeshNodal(&ne, &nn, eptr, eind, VWGT, VSIZE,
                                  &nparts, TPWGTS, options, &objval,
                                  epart, npart);

    // =====================================
    // Print Metis results.
    // =====================================

    switch (err)
    {
    case METIS_OK:
        break;
    case METIS_ERROR_INPUT:
        printf("error input\n");
        return 1;
        break;
    case METIS_ERROR_MEMORY:
        printf("error memory\n");
        return 1;
        break;
    case METIS_ERROR:
        printf("error\n");
        return 1;
        break;
    default:
        printf("Unexpected return value: %d\n", err);
        return 1;
        break;
    }

    printf("objval: %"PRId64"\n", objval);

    printf("epart: ");
    idx_t i;
    for (i= 0 ; i < ne ; i++)
        printf("%"PRId64" ", epart[i]);
    printf("\n");

    printf("npart: ");
    for (i= 0 ; i < nn ; i++)
        printf("%"PRId64" ", npart[i]);
    printf("\n");

    printf("%lu\n", sizeof(idx_t));

    return 0;
}
