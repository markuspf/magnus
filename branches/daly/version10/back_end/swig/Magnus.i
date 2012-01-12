/*
Magnus.i
$Id$

This is the main interface file. It doesn't do anything except include
a bunch of other files...

Peter Brinkmann, 05/18/2001
brinkman@math.uiuc.edu
*/
%module Magnus

%include Magnustypes.i

///magnus/back_end/general
%include Type.i
%include Trichotomy.i
%include DArray.i
%include RandomNumbers.i
%include Timer.i
%include MagnusHome.i

///magnus/back_end/FSA
%include FSA.i
%include DFSA.i
///magnus/back_end/Elt
%include Generator.i
%include Elt.i
%include Word.i
%include NormalRandomWord.i
%include AbelianWord.i


///magnus/back_end/KB
%include WordOrder.i
%include GenMult.i
%include DiffMachine.i
%include KBMachine.i
%include KBmagPackage.i
%include RKBPackage.i


///magnus/back_end/Group
%include Group.i
%include FGGroup.i
%include FPGroup.i
%include FreeGroup.i
%include Homology.i
%include SymmetricRelators.i
%include ShortenByRelators.i
%include ORWordProblem.i
%include RandomPrimitiveElement.i
%include PowerSeriesWP.i
%include FreeByCyclic.i
%include WordEnumerator.i
%include TTP.i
%include TietzeTrekker.i
%include RipsConstruction.i
%include RandomMSCGroup.i
%include Products.i
%include MSCGroup.i
%include MSCGConjugacyProblem.i
%include GroupFastChecks.i
%include AbelianGroup.i
%include AbelianSGPresentation.i
%include AbelianInfinitenessProblem.i
%include AbelianEquations.i
%include GeneralWhitehead.i


////magnus/back_end/Map
%include Map.i
%include RandomAutoInFree.i
%include Endomorphism.i
%include Automorphism.i


////magnus/back_end/Equations
%include DGESolver.i
%include QEqnSolutions.i


///magnus/back_end/NilpotentGroup
%include Letter.i
%include BasicCommutators.i
%include LCSQuotients.i
%include MalcevSet.i
%include NilpotentCollector.i
%include NilpCollectors.i
%include NilpotentGroup.i
%include PolyWord.i
%include PolyWordIterator.i
%include Presentation.i
%include SGOfNilpotentGroup.i
%include SubgroupBasis.i


////magnus/back_end/Subgroup
%include Subgroup.i
%include SGofFreeGroup.i
%include SubgroupGraph.i
%include DecomposeInSubgroup.i
%include GraphConjugacyProblem.i
%include PresentationsOfSubgroup.i
%include TurnerProperSubgroupEnumerator.i

/////magnus/back_end/AProducts
%include Range.i
%include AP-fixups.i
%include  HNNExtension.i
%include  APofFreeGroups.i
%include  APwithOneRelator.i
%include  HNNExtOfFreeGroup.i
%include  HNNExtOfORGroup.i
%include MagnusBreakdown.i
%include OneRelatorGroup.i
%include OneRelatorGroupWithTorsion.i
%include ORProblems.i
%include SubgroupOfOneRelatorGroup.i  ///abstract xxwundone 


/////magnus/back_end/Todd-Coxeter
%include CosetEnumerator.i
%include HavasTC.i

//////magnus/back_end/Genetic
%include GACPforORGSolver.i
%include GAEquationSolver.i
%include TwoCommSolver.i
%include ACConfig.i
%include ACGA.i
%include Config.i
%include GAIsPartOfBasis.i
%include GASubgroup.i
%include GAWP.i



////magnus/back_end/Matrix
%include Matrix.i
%include GaussTransformation.i
%include MatrixComputations.i
%include RandomMatrix.i

