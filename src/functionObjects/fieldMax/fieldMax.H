/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
13-05-2020 Jeff Heylmun:    | Store maxes over all times
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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

Class
    Foam::functionObjects::fieldMax

Description
    Tracks the maximum value of a given list of field names at every cell. If
    Adaptive mesh refinement is used, the maximum value may decrease due to
    volume averaging.

    Example of function object specification:
    \verbatim
    max
    {
        type                fieldMax;
        libs                ("libblastFunctionObjects.so");

        writeControl    writeTime;
        restartOnRestart    false;

        fields
        (
            p
            T
        );
    }
    \endverbatim

Usage
    \table
        Property          | Description               | Required | Default
        restartOnRestart  | Restart the averaging on restart |no | no
        fields            | Name of  fields           | yes
    \endtable


See also
    Foam::functionObjects::fvMeshFunctionObject

SourceFiles
    fieldMax.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_fieldMax_H
#define functionObjects_fieldMax_H

#include "fvMeshFunctionObject.H"
#include "mapPolyMesh.H"
#include "PtrListDictionary.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{


/*---------------------------------------------------------------------------*\
                        Class fieldMax Declaration
\*---------------------------------------------------------------------------*/

class fieldMax
:
    public fvMeshFunctionObject
{
protected:

    // Protected data


        //- Restart the averaging process on output
        Switch restartOnRestart_;

        //- Name of fields
        wordList fieldNames_;

        //- Name of max fields
        wordList maxFieldNames_;


        labelList* cellMap_;
        labelList* rCellMap_;

        //- Old field values
        PtrListDictionary<volScalarField> vScalarFields_;
        PtrListDictionary<volVectorField> vVectorFields_;
        PtrListDictionary<volSphericalTensorField> vSphericalTensorFields_;
        PtrListDictionary<volSymmTensorField> vSymmTensorFields_;
        PtrListDictionary<volTensorField> vTensorFields_;

        PtrListDictionary<surfaceScalarField> sScalarFields_;
        PtrListDictionary<surfaceVectorField> sVectorFields_;
        PtrListDictionary<surfaceSphericalTensorField> sSphericalTensorFields_;
        PtrListDictionary<surfaceSymmTensorField> sSymmTensorFields_;
        PtrListDictionary<surfaceTensorField> sTensorFields_;


        //- Store old fields
        template<class Type>
        void mapMax
        (
            const word& fName,
            const mapPolyMesh&,
            const PtrListDictionary<Type>& old
        );

        //- Update field max
        template<class Type>
        void createMax
        (
            const word& fieldName,
            const word& maxFieldName
        );

        //- Create old field
        template<class Type>
        void createOld
        (
            const word& maxFieldName,
            PtrListDictionary<Type>& old
        );

        //- Update field max
        template<class Type>
        void updateMax
        (
            const word& fieldName,
            const word& maxFieldName,
            PtrListDictionary<Type>& old
        );

        //- Write field max
        template<class type>
        void writeField(const word& fieldName);

        //- Set all old fields and cell maps
        void setOldFields(const mapPolyMesh& mpm);

        //- Clear all old field and cell maps
        void clearOldFields();

public:

    //- Runtime type information
    TypeName("fieldMax");


    // Constructors

        //- Construct from Time and dictionary
        fieldMax
        (
            const word& name,
            const Time& runTime,
            const dictionary&
        );

        //- Disallow default bitwise copy construction
        fieldMax(const fieldMax&) = delete;


    //- Destructor
    virtual ~fieldMax();


    // Member Functions

        //- Read the field average data
        virtual bool read(const dictionary&);

        //- Update mesh and store map
        virtual void updateMesh(const mapPolyMesh& mpm);

        //- Calculate the field averages
        virtual bool execute();

        //- Write the field averages
        virtual bool write();


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const fieldMax&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "fieldMaxTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
