(set-logic AUFLIA)
(set-info :source | Burns mutual exclusion protocol. This is a benchmark of the haRVey theorem prover. It was translated to SMT-LIB by Leonardo  de Moura |)
(set-info :smt-lib-version 2.0)
(set-info :category "industrial")
(set-info :status unsat)
(declare-fun s_0 (Int) Bool)
(declare-fun s_1 (Int) Bool)
(declare-fun s_2 (Int) Bool)
(declare-fun s_3 (Int) Bool)
(declare-fun s_4 (Int) Bool)
(declare-fun s_5 (Int) Bool)
(declare-fun s (Int Int) Bool)
(declare-fun flag (Int) Bool)
(declare-fun p () Int)
(declare-fun q () Int)
(assert (not (=> (and (forall ((?p Int)) (=> (not (flag ?p)) (or (s_0 ?p) (s_1 ?p) (s_2 ?p)))) (forall ((?p Int)) (forall ((?q Int)) (=> (s_2 ?p) (not (s ?p ?q))))) (forall ((?p Int)) (forall ((?q Int)) (=> (and (< ?q ?p) (flag ?q) (or (s_5 ?p) (s_4 ?p) (and (s_3 ?p) (s ?p ?q)))) (and (not (s_5 ?q)) (not (and (s_4 ?q) (s ?q ?p))))))) (forall ((?p Int)) (=> (s_0 ?p) (not (or (s_1 ?p) (s_2 ?p) (s_3 ?p) (s_4 ?p) (s_5 ?p))))) (forall ((?p Int)) (=> (s_1 ?p) (not (or (s_2 ?p) (s_3 ?p) (s_4 ?p) (s_5 ?p))))) (forall ((?p Int)) (=> (s_2 ?p) (not (or (s_3 ?p) (s_4 ?p) (s_5 ?p))))) (forall ((?p Int)) (=> (s_3 ?p) (not (or (s_4 ?p) (s_5 ?p))))) (forall ((?p Int)) (=> (s_4 ?p) (not (s_5 ?p)))) (forall ((?r Int)) (let ((?v_0 (not (= ?r p)))) (=> (s_0 ?r) (not (or (s_1 ?r) (s_2 ?r) (s_3 ?r) (=> ?v_0 (and ?v_0 (=> ?v_0 (s_4 ?r)))) (s_5 ?r)))))) (forall ((?r Int)) (let ((?v_1 (not (= ?r p)))) (=> (s_1 ?r) (not (or (s_2 ?r) (s_3 ?r) (=> ?v_1 (and ?v_1 (=> ?v_1 (s_4 ?r)))) (s_5 ?r)))))) (forall ((?r Int)) (let ((?v_2 (not (= ?r p)))) (=> (s_2 ?r) (not (or (s_3 ?r) (=> ?v_2 (and ?v_2 (=> ?v_2 (s_4 ?r)))) (s_5 ?r)))))) (forall ((?r Int)) (let ((?v_3 (not (= ?r p)))) (=> (s_3 ?r) (not (or (=> ?v_3 (and ?v_3 (=> ?v_3 (s_4 ?r)))) (s_5 ?r)))))) (forall ((?r Int)) (let ((?v_4 (not (= ?r p)))) (=> (=> ?v_4 (and ?v_4 (=> ?v_4 (s_4 ?r)))) (not (s_5 ?r))))) (s_4 p) (not (s p q)) (< p q) (not (flag q))) (and (forall ((?t Int)) (=> (not (flag ?t)) (or (s_0 ?t) (s_1 ?t) (s_2 ?t)))) (forall ((?t Int)) (forall ((?r Int)) (=> (s_2 ?t) (not (=> (not (and (= ?r q) (= ?t p))) (s ?t ?r)))))) (forall ((?t Int)) (forall ((?r Int)) (let ((?v_6 (= ?t p))) (let ((?v_5 (not ?v_6)) (?v_8 (= ?r p))) (let ((?v_7 (not ?v_8))) (=> (and (< ?r ?t) (flag ?r) (or (s_5 ?t) (=> ?v_5 (and ?v_5 (=> ?v_5 (s_4 ?t)))) (and (s_3 ?t) (=> (not (and (= ?r q) ?v_6)) (s ?t ?r))))) (and (not (s_5 ?r)) (not (and (=> ?v_7 (and ?v_7 (=> ?v_7 (s_4 ?r)))) (=> (not (and (= ?t q) ?v_8)) (s ?r ?t)))))))))))))))
(check-sat)
(exit)
