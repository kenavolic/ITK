/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MeshTraits.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//  Software Guide : BeginLatex
//
//  This section illustrates the full power of 
//  \href{http://www.boost.org/more/generic_programming.html}{Generic
//  Programming}.  This is sometimes perceived as \emph{too much of a good
//  thing}! 
//
//  The toolkit has been designed to offer flexibility yet keeping the
//  complexity of the code to a moderate level. This is achieved in the Mesh by
//  hidding most of its parameters and defining reasonable defaults for them.
// 
//  The generic concept of a mesh integrate many different elements. It is
//  possible in principle to use independent types for every one of such
//  elements. The mechanism used in generic programming for specifying the many
//  different types involved in a concept is called \emph{traits}. They are
//  basically the list of all types that interact with the current class.
//
//  The \doxygen{Mesh} is templated over three parameters. So far only two of
//  them have been discussed, namely the \code{PixelType} and the
//  \code{Dimension}. The third parameter is a class providing the set of
//  traits required by the mesh. When the third parameter is omitted a default
//  class is used. This default class is the \doxygen{DefaultStaticMeshTraits}.
//  If you want to customize the types used by the mesh, the way to proceed is
//  to modify the default traits and provide them as the third parameter of the
//  Mesh class instantiation.
//
//  There are two ways of achieving this. The first is to use the existing
//  \doxygen{DefaultStaticMeshTraits} class. This class is itself templated
//  over six parameters.  Customizing those parameters could provide enough
//  flexibility to define a very specific kind of mesh. The second way is to
//  write a traits class from scratch, in which case the easiest way to proceed
//  is to copy the \code{itkDefaultStaticMeshTraits} into another file and edit
//  its content. Only the first approach is illustrated here. The second is
//  discouraged unless you are familiar with Generic Programming, feel
//  comfortable with C++ templates and have access to an abundant supply of
//  coffee. 
//
//  The first step in customizing the mesh is to include the header file of the
//  \doxygen{Mesh} and its StaticTraits.
//  
//  \index{itk::DefaultStaticMeshTraits!Header}
//
//  Software Guide : EndLatex 



// Software Guide : BeginCodeSnippet
#include "itkMesh.h"
#include "itkDefaultStaticMeshTraits.h"
// Software Guide : EndCodeSnippet

#include "itkLineCell.h"
#include "itkVector.h"
#include "itkMatrix.h"

int main()
{


  //  Software Guide : BeginLatex
  //  
  //  Then the MeshTraits class is instantiated by selecting the types of each
  //  one of its six template arguments. They are in order
  // 
  //  \begin{itemize}
  //  \item PixelType The type associated with every point
  //  \item PointDimension The dimension of the space in which the mesh is embedded
  //  \item MaxTopologicalDimension The highest dimension of the mesh cells 
  //  \item CoordinateRepresentation The type used to represent space coordinates
  //  \item InterpolationWeigh The type used to represent interpolation weights
  //  \item CellDataType The type associated with every cell
  //
  //  \end{itemize}
  //
  //  Let's define types and values for each one of those elements. For example
  //  the following code will use points in $3D$ space as nodes of the
  //  \doxygen{Mesh}. The maximum dimension of the cells will be 2 which means
  //  that this is a 2D manifold better know as a \emph{surface}. The data type
  //  associated with points is defined to be a four dimensional vector. This
  //  type could represent values of membership for a four-classes segmentation
  //  method.  The value selected for the cells are $4\times3$ matrices which could
  //  have for example the derivative of the membership values with respect to
  //  coordinates in space. Finally a \code{double} type is selected for
  //  representing space coordinates on the mesh points and also for the weight
  //  used for interpolating values.
  //
  //  \index{itk::DefaultStaticMeshTraits!Instantiation}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  const unsigned int PointDimension = 3;
  const unsigned int MaxTopologicalDimension = 2;

  typedef itk::Vector<double,4>                  PixelType;
  typedef itk::Matrix<double,4,3>                CellDataType;

  typedef double CoordinateType;
  typedef double InterpolationWeightType;

  typedef itk::DefaultStaticMeshTraits< 
            PixelType, PointDimension, MaxTopologicalDimension,
            CoordinateType, InterpolationWeightType, CellDataType > MeshTraits;
                            
  typedef itk::Mesh< PixelType, PointDimension, MeshTraits > MeshType;
  // Software Guide : EndCodeSnippet



  //  Software Guide : BeginLatex
  //
  //  The \doxygen{LineCell} type can now be instantiated using the traits
  //  taken from the Mesh.  
  //
  //  \index{itk::LineCell!Instantiation}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef MeshType::CellType                CellType;
  typedef itk::LineCell< CellType >         LineType;
  // Software Guide : EndCodeSnippet




  //  Software Guide : BeginLatex
  //
  //  Let's now create an \doxygen{Mesh} and insert some points on it. Note
  //  that the dimension of the points matches the dimension of the Mesh. Here
  //  we insert a sequence of points that look like a plot of the $log()$
  //  function.
  //
  //  \index{itk::Mesh!New()}
  //  \index{itk::Mesh!SetPoint()}
  //  \index{itk::Mesh!PointType}
  //  \index{itk::Mesh!Pointer}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  MeshType::Pointer  mesh = MeshType::New();

  typedef MeshType::PointType PointType;
  PointType point;

  const unsigned int numberOfPoints = 10;
  for(unsigned int id=0; id<numberOfPoints; id++) 
    {
    point[0];   // Initialize points here
    point[1];
    point[2];
    mesh->SetPoint( id, point );
    }
  // Software Guide : EndCodeSnippet





  //  Software Guide : BeginLatex
  //
  //  A set of line cells is created and associated with the existing points by
  //  using point identifiers. In this simple case, the point identifiers can
  //  be deduced from cell identifiers since the line cells are ordered in the
  //  same way.
  //
  //  \index{itk::AutoPointer!TakeOwnership()}
  //  \index{CellAutoPointer!TakeOwnership()}
  //  \index{CellType!creation}
  //  \index{itk::Mesh!SetCell()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  CellType::CellAutoPointer line;
  const unsigned int numberOfCells = numberOfPoints-1;
  for(unsigned int cellId=0; cellId<numberOfCells; cellId++)
    {
    line.TakeOwnership(  new LineType  );
    line->SetPointId( 0, cellId   ); // first point 
    line->SetPointId( 1, cellId+1 ); // second point
    mesh->SetCell( cellId, line );   // insert the cell 
    }
  // Software Guide : EndCodeSnippet


  std::cout << "Points = " << mesh->GetNumberOfPoints() << std::endl;
  std::cout << "Cells  = " << mesh->GetNumberOfCells()  << std::endl;



  //  Software Guide : BeginLatex
  //
  //  Data associated with cells is inserted in the \doxygen{Mesh} by using
  //  the \code{SetCellData()} method.  It requires the user to provide an
  //  identifier and the value to be inserted. The identifier should match one
  //  of the inserted cells. In this simple example, the square of the cell
  //  identifier is used as cell data. Note the use of \code{static\_cast} to
  //  \code{PixelType} in the assignent.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  for(unsigned int cellId=0; cellId<numberOfCells; cellId++)
    {
    CellDataType value;
    mesh->SetCellData( cellId, value );
    }

  // Software Guide : EndCodeSnippet




  //  Software Guide : BeginLatex
  //
  //  Cell data can be read from the \doxygen{Mesh} with the
  //  \code{GetCellData()} method. It requires the user to provide the
  //  identifier of the cell for which the data is to be retrieved. The user
  //  should provide also a valid pointer to a location where the data can be
  //  copied.
  //
  //  \index{itk::Mesh!GetCellData()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  for(unsigned int cellId=0; cellId<numberOfCells; cellId++)
    {
    CellDataType value;
    mesh->GetCellData( cellId, &value );
    std::cout << "Cell " << cellId << " = " << value << std::endl;
                       
    }

  // Software Guide : EndCodeSnippet





  //  Software Guide : BeginLatex
  //
  //  Neither \code{SetCellData()} or \code{GetCellData()} are efficient ways
  //  to access cell data. Massive access to cell data can be achieved
  //  efficently by using the Iterators built-in the CellDataContainer. 

  // Software Guide : BeginCodeSnippet
  typedef MeshType::CellDataContainer::ConstIterator CellDataIterator;
  // Software Guide : EndCodeSnippet







  //  Software Guide : BeginLatex
  //
  //  Note that the \code{ConstIterator} is used here because the data is only
  //  going to be read.  This approach is exactly the same already illustrated
  //  for getting access to point data. The iterator to the first cell data
  //  item can be obtained with the \code{Begin()} method of the
  //  CellDataContainer. The past-end iterator is returned by the \code{End()}
  //  method. The cell data container itself can be obtained from the mesh with
  //  the method \code{GetCellData()}.
  //
  //  \index{itk::Mesh!Iterating cell data}
  //  \index{itk::Mesh!GetCellData()}
  //  \index{CellDataContainer!Begin()}
  //  \index{CellDataContainer!End()}
  //  \index{CellDataContainer!Iterator}
  //  \index{CellDataContainer!ConstIterator}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  CellDataIterator cellDataIterator  = mesh->GetCellData()->Begin();  
  CellDataIterator end               = mesh->GetCellData()->End();  
  // Software Guide : EndCodeSnippet







  //  Software Guide : BeginLatex
  //
  //  Finally a standard loop is used to iterate over all the cell data
  //  entries. Note the use of the \code{Value()} method used to get the actual
  //  value of the data entry. \code{PixelType} elements are returned by copy.
  //
  //  \index{CellDataIterator!Value()}
  //  \index{CellDataIterator!increment}
  //
  //  Software Guide : EndLatex 


  // Software Guide : BeginCodeSnippet
  while( cellDataIterator != end ) {
    CellDataType cellValue = cellDataIterator.Value();
    std::cout << cellValue << std::endl;
    ++cellDataIterator;
    }
  // Software Guide : EndCodeSnippet


  return 0;

}

