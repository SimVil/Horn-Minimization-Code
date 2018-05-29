# Readme: Horn Minimization

This repository contains implementation of Horn minimization algorithms. It
relies on the code of https://github.com/yazevnul/fcai for FCA and closure
testing purposes. We will be interested in implementation details, uses, possible improvements and so on. One can be interested in reading the pdf version of this
README, providing some theoretical recap.

### Algorithms

We implemented 5 algorithms:

  * `MinCover`: an algorithm found in **[7, 9, 14]**,
  * `DuquenneMin`: being a variation of `MinCover` found in **[8]**,
  * `MaierMin`: a procedure coming from database community, see **[12, 13]**,
  * `BercziMin`: based on hypergraph representation of logical Horn formula (**[6]**)
  * `AFP`: an algorithm derived from query learning, further details in **[1, 2]**

For closure algorithms, please refer to **[5, 9]**

### MinCover


## Main dish: implementation

## References

 - **[1]** Angluin, D., Frazier, M., and Pitt, L. *Learning conjunctions of Horn clauses*. Machine Learning 9, 2 (July 1992), 147–164.

 - **[2]** Arias, M., and Balcázar, J. L. *Canonical Horn Representations and Query Learning*. In Algorithmic Learning Theory (Berlin, Heidelberg, 2009), R. Gavaldà, G. Lugosi, T. Zeugmann, and S. Zilles, Eds., Springer Berlin Heidelberg, pp. 156–170.

 - **[3]** Ausiello, G., D’Atri, A., and Saccà, D. *Graph Algorithms for Functional Dependency Manipulation*. J. ACM 30, 4 (Oct. 1983), 752–766.

 - **[4]** Ausiello, G., D’Atri, A., and Saccá, D. *Minimal Representation of Directed Hypergraphs*. SIAM J. Comput. 15, 2 (May 1986), 418–431.

 - **[5]** Beeri, C., and Bernstein, P. A. *Computational Problems Related to the Design of Normal Form Relational Schemas*. ACM Trans. Database Syst. 4, 1 (Mar. 1979), 30–59.

 - **[6]** Bérczi, K., and Bérczi-Kovács, E. R. *Directed hypergraphs and Horn minimization*. Information Processing Letters 128 (2017), 32 – 37.

 - **[7]** Day, A. *The Lattice Theory of Functional Dependencies and Normal Decompositions*. International Journal of Algebra and Computation (1992).

 - **[8]** Duquenne, V. *Some variations on Alan Day’s algorithm for calculating canonical basis of implications*. In Concept Lattices and their Applications (CLA) (Montpellier, France, 2007), pp. 17–25.

 - **[9]** Ganter, B., and Obiedkov, S. *Conceptual Exploration*. Springer, 2016.

 - **[10]** Ganter, B., and Wille, R. *Formal Concept Analysis: Mathematical Foundations*. Springer-Verlag, Berlin Heidelberg, 1999.

 - **[11]** Guigues, J., and Duquenne, V. *Familles minimales d’implications informatives résultant d’un tableau de données binaires*. Mathématiques et Sciences Humaines 95 (1986), 5–18.

 - **[12]** Maier, D. *Minimum Covers in Relational Database Model*. J. ACM 27, 4 (1980), 664 – 674.

 - **[13]** Maier, D. *Theory of Relational Databases*. Computer Science Pr, 1983.

 - **[14]** Wild, M. *Implicational bases for finite closure systems*. Informatik-Bericht 89/3, Institut fuer Informatik (Jan. 1989).

 - **[15]** Wild, M. *A Theory of Finite Closure Spaces Based on Implications*. Advances in Mathematics 108, 1 (Sept. 1994), 118–139.

 - **[16]** Wild, M. *Computations with finite closure systems and implications*. In Computing and Combinatorics (Aug. 1995), Lecture Notes in Computer Science, Springer, Berlin, Heidelberg, pp. 111–120.
