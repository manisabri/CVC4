/*********************                                                        */
/*! \file theory_black.h
 ** \verbatim
 ** Original author: taking
 ** Major contributors: barrett, mdeters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 prototype.
 ** Copyright (c) 2009, 2010, 2011  The Analysis of Computer Systems Group (ACSys)
 ** Courant Institute of Mathematical Sciences
 ** New York University
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Black box testing of CVC4::theory::Theory.
 **
 ** Black box testing of CVC4::theory::Theory.
 **/

#include <cxxtest/TestSuite.h>

#include "theory/theory.h"
#include "expr/node.h"
#include "expr/node_manager.h"
#include "context/context.h"

#include <vector>

using namespace CVC4;
using namespace CVC4::theory;
using namespace CVC4::expr;
using namespace CVC4::context;

using namespace std;

/**
 * Very basic OutputChannel for testing simple Theory Behaviour.
 * Stores a call sequence for the output channel
 */
enum OutputChannelCallType{CONFLICT, PROPAGATE, LEMMA, EXPLANATION};
class TestOutputChannel : public OutputChannel {
private:
  void push(OutputChannelCallType call, TNode n) {
    d_callHistory.push_back(make_pair(call, n));
  }

public:
  vector< pair<OutputChannelCallType, Node> > d_callHistory;

  TestOutputChannel() {}

  ~TestOutputChannel() {}

  void safePoint() throw(Interrupted, AssertionException) {}

  void conflict(TNode n)
    throw(AssertionException) {
    push(CONFLICT, n);
  }

  bool propagate(TNode n)
    throw(AssertionException) {
    push(PROPAGATE, n);
    return true;
  }

  void propagateAsDecision(TNode n)
    throw(AssertionException) {
    // ignore
  }

  LemmaStatus lemma(TNode n, bool removable)
    throw(AssertionException) {
    push(LEMMA, n);
    return LemmaStatus(Node::null(), 0);
  }

  void requirePhase(TNode, bool)
    throw(Interrupted, AssertionException) {
    Unreachable();
  }

  bool flipDecision()
    throw(Interrupted, AssertionException) {
    Unreachable();
  }

  void setIncomplete()
    throw(AssertionException) {
    Unreachable();
  }

  void clear() {
    d_callHistory.clear();
  }

  Node getIthNode(int i) {
    Node tmp = (d_callHistory[i]).second;
    return tmp;
  }

  OutputChannelCallType getIthCallType(int i) {
    return (d_callHistory[i]).first;
  }

  unsigned getNumCalls() {
    return d_callHistory.size();
  }
};

class DummyTheory : public Theory {
public:
  set<Node> d_registered;
  vector<Node> d_getSequence;

  DummyTheory(Context* ctxt, UserContext* uctxt, OutputChannel& out, Valuation valuation, const LogicInfo& logicInfo, QuantifiersEngine* qe) :
    Theory(theory::THEORY_BUILTIN, ctxt, uctxt, out, valuation, logicInfo, qe) {
  }

  void registerTerm(TNode n) {
    // check that we registerTerm() a term only once
    TS_ASSERT(d_registered.find(n) == d_registered.end());

    for(TNode::iterator i = n.begin(); i != n.end(); ++i) {
      // check that registerTerm() is called in reverse topological order
      TS_ASSERT(d_registered.find(*i) != d_registered.end());
    }

    d_registered.insert(n);
  }

  Node getWrapper() {
    Node n = get();
    d_getSequence.push_back(n);
    return n;
  }

  bool doneWrapper() {
    return done();
  }

  void check(Effort e) {
    while(!done()) {
      getWrapper();
    }
  }

  void presolve() {
    Unimplemented();
  }
  void preRegisterTerm(TNode n) {}
  void propagate(Effort level) {}
  void explain(TNode n, Effort level) {}
  Node getValue(TNode n) { return Node::null(); }
  string identify() const { return "DummyTheory"; }
};/* class DummyTheory */

class TheoryBlack : public CxxTest::TestSuite {

  Context* d_ctxt;
  UserContext* d_uctxt;
  NodeManager* d_nm;
  NodeManagerScope* d_scope;
  LogicInfo* d_logicInfo;

  TestOutputChannel d_outputChannel;

  DummyTheory* d_dummy;

  Node atom0;
  Node atom1;

public:

  void setUp() {
    d_ctxt = new Context();
    d_uctxt = new UserContext();
    d_nm = new NodeManager(d_ctxt, NULL);
    d_scope = new NodeManagerScope(d_nm);
    d_logicInfo = new LogicInfo();
    d_logicInfo->lock();
    d_dummy = new DummyTheory(d_ctxt, d_uctxt, d_outputChannel, Valuation(NULL), *d_logicInfo, NULL);
    d_outputChannel.clear();
    atom0 = d_nm->mkConst(true);
    atom1 = d_nm->mkConst(false);
  }

  void tearDown() {
    atom1 = Node::null();
    atom0 = Node::null();
    delete d_dummy;
    delete d_logicInfo;
    delete d_scope;
    delete d_nm;
    delete d_uctxt;
    delete d_ctxt;
  }

  void testEffort(){
    Theory::Effort s = Theory::EFFORT_STANDARD;
    Theory::Effort f = Theory::EFFORT_FULL;

    TS_ASSERT( Theory::standardEffortOnly(s));
    TS_ASSERT(!Theory::standardEffortOnly(f));

    TS_ASSERT(!Theory::fullEffort(s));
    TS_ASSERT( Theory::fullEffort(f));

    TS_ASSERT( Theory::standardEffortOrMore(s));
    TS_ASSERT( Theory::standardEffortOrMore(f));
  }

  void testDone() {
    TS_ASSERT(d_dummy->doneWrapper());

    d_dummy->assertFact(atom0, true);
    d_dummy->assertFact(atom1, true);

    TS_ASSERT(!d_dummy->doneWrapper());

    d_dummy->check(Theory::EFFORT_FULL);

    TS_ASSERT(d_dummy->doneWrapper());
  }

  // FIXME: move this to theory_engine test?
//   void testRegisterTerm() {
//     TS_ASSERT(d_dummy->doneWrapper());

//     TypeNode typeX = d_nm->booleanType();
//     TypeNode typeF = d_nm->mkFunctionType(typeX, typeX);

//     Node x = d_nm->mkVar("x",typeX);
//     Node f = d_nm->mkVar("f",typeF);
//     Node f_x = d_nm->mkNode(kind::APPLY_UF, f, x);
//     Node f_f_x = d_nm->mkNode(kind::APPLY_UF, f, f_x);
//     Node f_x_eq_x = f_x.eqNode(x);
//     Node x_eq_f_f_x = x.eqNode(f_f_x);

//     d_dummy->assertFact(f_x_eq_x);
//     d_dummy->assertFact(x_eq_f_f_x);

//     Node got = d_dummy->getWrapper();

//     TS_ASSERT_EQUALS(got, f_x_eq_x);

//     TS_ASSERT_EQUALS(5u, d_dummy->d_registered.size());
//     TS_ASSERT(d_dummy->d_registered.find(x) != d_dummy->d_registered.end());
//     TS_ASSERT(d_dummy->d_registered.find(f) != d_dummy->d_registered.end());
//     TS_ASSERT(d_dummy->d_registered.find(f_x) != d_dummy->d_registered.end());
//     TS_ASSERT(d_dummy->d_registered.find(f_x_eq_x) != d_dummy->d_registered.end());
//     TS_ASSERT(d_dummy->d_registered.find(d_nm->operatorOf(kind::EQUAL)) != d_dummy->d_registered.end());
//     TS_ASSERT(d_dummy->d_registered.find(f_f_x) == d_dummy->d_registered.end());
//     TS_ASSERT(d_dummy->d_registered.find(x_eq_f_f_x) == d_dummy->d_registered.end());

//     TS_ASSERT(!d_dummy->doneWrapper());

//     got = d_dummy->getWrapper();

//     TS_ASSERT_EQUALS(got, x_eq_f_f_x);

//     TS_ASSERT_EQUALS(7u, d_dummy->d_registered.size());
//     TS_ASSERT(d_dummy->d_registered.find(f_f_x) != d_dummy->d_registered.end());
//     TS_ASSERT(d_dummy->d_registered.find(x_eq_f_f_x) != d_dummy->d_registered.end());

//     TS_ASSERT(d_dummy->doneWrapper());
//   }

  void testOutputChannelAccessors() {
    /* void setOutputChannel(OutputChannel& out)  */
    /* OutputChannel& getOutputChannel() */

    TestOutputChannel theOtherChannel;

    TS_ASSERT_EQUALS(&(d_dummy->getOutputChannel()), &d_outputChannel);

    d_dummy->setOutputChannel(theOtherChannel);

    TS_ASSERT_EQUALS(&(d_dummy->getOutputChannel()), &theOtherChannel);

    const OutputChannel& oc = d_dummy->getOutputChannel();

    TS_ASSERT_EQUALS(&oc, &theOtherChannel);
  }

  void testOutputChannel() {
    Node n = atom0.orNode(atom1);
    d_outputChannel.lemma(n, false);
    d_outputChannel.split(atom0);
    Node s = atom0.orNode(atom0.notNode());
    TS_ASSERT_EQUALS(d_outputChannel.d_callHistory.size(), 2u);
    TS_ASSERT_EQUALS(d_outputChannel.d_callHistory[0], make_pair(LEMMA, n));
    TS_ASSERT_EQUALS(d_outputChannel.d_callHistory[1], make_pair(LEMMA, s));
    d_outputChannel.d_callHistory.clear();
  }
};
