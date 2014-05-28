// Function: Compute derivatives of states
// inputs:   m_mass m_addedMass m_uvw m_pqr m_xyz m_qRB m_externalForces
// outputs:  m_uvw_dot m_pqr_dot m_xyz_dot m_qRB_dot
void Ship6DOF::ComputeStateDerivatives()
{
    // inverse of total mass
    Matrix totalMass=m_mass+m_addedMass;
    Matrix invMass=totalMass.Inv();
    Matrix s1;
    Matrix s2;
    // coriolis and centripetal forces
    // Hydrodynamic Forces And Moments equation (2.125) chapter 2.4.1 page 36 C_A(\nu)
    Matrix m11=totalMass.ExtractSubMatrix(0,0,3,3);
    Matrix m12=totalMass.ExtractSubMatrix(0,3,3,3);
    Matrix m21=totalMass.ExtractSubMatrix(3,0,3,3);
    Matrix m22=totalMass.ExtractSubMatrix(3,3,3,3);
    s1=MakeSkewSymmetric(m11*m_uvw+m12*m_pqr);
    s2=MakeSkewSymmetric(m21*m_uvw+m22*m_pqr);

    Matrix coriolisForces(6,1);
    coriolisForces.InsertSubMatrix(s1*m_pqr,0,0);
    coriolisForces.InsertSubMatrix(s1*m_uvw+s2*m_pqr,3,0);

    // Dynamic Equations
    Matrix totalForces=m_externalForces+coriolisForces;
    Matrix nuDot=invMass*totalForces;
    m_uvw_dot=nuDot.ExtractSubMatrix(0,0,3,1);
    m_pqr_dot=nuDot.ExtractSubMatrix(3,0,3,1);

    // Kinematic Equations
    m_xyz_dot=m_qRB.GetDCM() * m_uvw;
    m_qRB_dot=0.5* (m_qRB * Quaternion(0.0,m_pqr[0][0],m_pqr[1][0],m_pqr[2][0])).AsFourVector();
} 
