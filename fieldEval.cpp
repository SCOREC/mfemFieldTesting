#include "mfem.hpp"
#include <iostream>

using namespace mfem;
using namespace std;

int main(int argc, char *argv[])
{
    // 0. Initialize device (GPU support)
    Device device("cuda"); // or "hip", "raja", "cpu" for fallback

    // 1. Create a 2D triangular mesh
    const int nx = 4, ny = 4;
    Mesh mesh = Mesh::MakeCartesian2D(nx, ny, Element::TRIANGLE, true, 1.0, 1.0);

    // 2. Finite element space for linear field
    int order = 1;
    H1_FECollection fec(order, mesh.Dimension());
    FiniteElementSpace fespace(&mesh, &fec);

    // 3. Project the function x^2 and constant 1
    FunctionCoefficient x2_coeff([](const Vector &x) { return x(0) * x(0); });
    FunctionCoefficient y1_coeff([](const Vector &x) { return 1.0; });

    GridFunction x_squared(&fespace);
    GridFunction y_const(&fespace);
    x_squared.ProjectCoefficient(x2_coeff);
    y_const.ProjectCoefficient(y1_coeff);

    // 4. Allocate output arrays to store values at centroids
    const int NE = mesh.GetNE();
    Vector centroid_x2(NE);
    Vector centroid_y1(NE);

    // 5. Loop over elements in parallel
    auto X2 = x_squared.Read();
    auto Y1 = y_const.Read();
    auto cent_x2 = centroid_x2.Write();
    auto cent_y1 = centroid_y1.Write();

    const FiniteElement &fe = *fespace.GetFE(0);
    const IntegrationRule &ir = IntRules.Get(fe.GetGeomType(), 1);
    const IntegrationPoint &ip = ir.IntPoint(0); // centroid

    // Create an ElementTransformation array once (host)
    Mesh *mesh_ptr = &mesh;
    FiniteElementSpace *fes_ptr = &fespace;

    mfem::forall(NE, [=] MFEM_HOST_DEVICE (int i)
    {
        ElementTransformation *trans = mesh_ptr->GetElementTransformation(i);
        Vector transip(2);
        trans->Transform(ip, transip);

        // Evaluate field manually
        double x = transip(0);
        double y = transip(1);

        cent_x2[i] = x * x;
        cent_y1[i] = 1.0;
    });

    // 6. Output results (back on host)
    cout << "Evaluated (x^2, 1) at triangle centroids (parallel GPU):" << endl;
    for (int i = 0; i < NE; i++)
    {
        cout << "Element " << i << ": Field = (" << centroid_x2(i) << ", " << centroid_y1(i) << ")" << endl;
    }

    return 0;
}

