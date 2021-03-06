/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2019 Synthetik Applied Technologies
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "RK2SSPTimeIntegrator.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace timeIntegrators
{
    defineTypeNameAndDebug(RK2SSP, 0);
    addToRunTimeSelectionTable(timeIntegrator, RK2SSP, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::timeIntegrators::RK2SSP::RK2SSP
(
    const fvMesh& mesh
)
:
    timeIntegrator(mesh)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::timeIntegrators::RK2SSP::~RK2SSP()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::timeIntegrators::RK2SSP::setODEFields(integrationSystem& system)
{
    system.setODEFields(2, {true, false}, {false, false});
}


void Foam::timeIntegrators::RK2SSP::integrate()
{
    // Update and store original fields
    forAll(systems_, i)
    {
        systems_[i].update();
        systems_[i].solve(1, {1.0}, {1.0});
    }

    // Update and store 1st step
    forAll(systems_, i)
    {
        systems_[i].update();
        systems_[i].solve(2, {0.5, 0.5}, {0.0, 0.5});
    }
}
// ************************************************************************* //
