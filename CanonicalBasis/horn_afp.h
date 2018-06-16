/// \file horn_afp.h
/// \author Simon Vilmin
/// \date 2018.
///
/// Defines AFP-Based Algorithm.

#ifndef ALGORITHMS_HORN_AFP_H
#define ALGORITHMS_HORN_AFP_H

#include "fca_closure_operators.h"
#include "fca_datastructures.h"
#include <list>
#include <deque>
#include <map>

namespace HORN {

    /// \brief Implementation of AFP-Based algorithm for implication systems.
    ///
    /// The function is a derivation of AFP algorithm of Angluin (HORN-1, 1992) "Learning conjunctions of Horn clauses"
    /// in which we do not have an oracle. The principle is to use a stack to store negative counter-ex generators.
    /// For each implication, the premise is considered as a generator. Depending on the case we may either refine
    /// the hypothesis Lc, or add new right-closed implications to it. Pseudo-code is as follows (simplified):
    /// \code
    ///     Initialize Lc to {};
    ///     Initialize a stack S to {};
    ///     for each A --> B of L do
    ///         store A in S
    ///         while S is not empty do
    ///             X := Closure(Head(S), Lc)
    ///             if X is a negative ex then
    ///                 found := false
    ///
    ///                 if we can find C --> D in Lc such that C n X is a negative ex then
    ///                     found := true
    ///                     replace C --> D by C n X --> L(C n X) in Lc
    ///                     X may still be a counter-ex, stack it into S
    ///
    ///                     if L(C n X) is smaller than D then
    ///                         it may be a counter-ex, stack it into S
    ///
    ///             if found = false then
    ///                 add X --> L(X) in Lc
    ///
    ///     return Lc
    /// \endcode
    ///
    /// \tparam ClosureOperator a closure operator to use.
    /// \param L input system to reduce.
    /// \param Lc Duquenne-Guigues base of L
    template <typename ClosureOperator>
    void AFPMinimization(const theory &L, theory &Lc);

    /// \brief Specified implementation of AFP, with LinClosure.
    ///
    /// Since we may use improvements of LinClosure, this function is a particular implementation of AFP based on the
    /// same pseudo-code as in the template version.
    ///
    /// \param L system to minimize.
    /// \param Lc Duquenne-Guigues representation of L.
    template <>
    void AFPMinimization<FCA::LinClosure>(const theory &L, theory &Lc);

}


// ==== template instanciation for general closure operators ====


// template definition of AFP
template <typename ClosureOperator>
void HORN::AFPMinimization(const theory &L, theory &Lc) {
    Lc.clear();

    if (L.empty())
        return;

    size_t attrNum = L.front().Premise().size();
    size_t implNum = L.size();

    int maximum_size = 0;

    // stack S
    std::list<FCA::BitSet> S;
    FCA::BitSet premise(attrNum), Lc_closure(attrNum), L_closure;
    FCA::BitSet C(attrNum), D(attrNum), M(attrNum);
    FCA::ImplicationInd copy;
    bool found;
    M.set();

    for (auto &impL : L){
        S.clear();
        S.emplace_back(impL.Premise());

        do{
            premise = S.back();
            S.pop_back();
            ClosureOperator::Apply(premise, Lc, Lc_closure);
            ClosureOperator::Apply(Lc_closure, L, L_closure);

            // if negative counter-example
            if(Lc_closure != L_closure){
                found = false;

                for (auto &impLc : Lc){
                    C = impLc.Premise();
                    C &= Lc_closure;  // C := A n X

                    // if we find a smaller counter-example
                    if (C != impLc.Premise()){
                        ClosureOperator::Apply(C, L, D);
                        if (C != D){
                            found = true;
                            copy = impLc;
                            impLc = FCA::ImplicationInd(C, D);   // replacement in K
                            Lc_closure |= D;
                            S.emplace_back(Lc_closure);  // in case X is still a counter-ex

                            if (copy.Conclusion() != D){
                                S.emplace_back(copy.Premise());  // if conclusions are not in the same class
                            }

                            if(S.size() > maximum_size){
                                maximum_size = (int) S.size();
                            }

                            break; // exit for --> not secured (assembly)
                        }
                    }

                }

                // if we did not find smaller counter-ex
                // add right-closed implication
                if (!found) {
                    M.reset();
                    ClosureOperator::Apply(Lc_closure, L, M);
                    Lc.emplace_back(FCA::ImplicationInd(Lc_closure, M));

                }

            }

        } while (!S.empty());

    }

//    if (maximum_size >= L.size())
//        std::cout << "stack maximum size: " << maximum_size << std::endl;

}


#endif //ALGORITHMS_HORN_AFP_H
