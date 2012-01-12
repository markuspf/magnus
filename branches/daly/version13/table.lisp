(defun row (prefix set)
 (let (result)
  (dolist (j set)
   (setq result (cons (format nil "~a~a" prefix j) result)))
   result))

(defun f ()
 (load "table.lisp")
 (main))

(defun shrink (word)
 (let ((result "") (retry nil))
  (cond 
   ((= (length word) 0))
   ((= (length word) 1) (setq result word))
   ((> (length word) 1)
    (dotimes (i (- (length word) 1))
     (cond 
      ((or 
        (and (eq (elt word i) #\a) (eq (elt word (+ i 1)) #\A))
        (and (eq (elt word i) #\A) (eq (elt word (+ i 1)) #\a))
        (and (eq (elt word i) #\b) (eq (elt word (+ i 1)) #\B))
        (and (eq (elt word i) #\B) (eq (elt word (+ i 1)) #\b)))
        (setf (elt word i) #\i)
        (setf (elt word (+ i 1)) #\i)
        (setq retry t))
      (t (setq result (concatenate 'string (string (elt word i)) result)))))
    (setq result 
     (concatenate 'string (string (elt word (- (length word) 1))) result))))
  (delete-if #'(lambda (x) (char= x #\i)) result) 
  (if (string-equal result "") (setq result "i"))
  (setq result (reverse result))
  (if retry (setq result (shrink result)))
  result))

(defun pinch (set)
 (let (result)
  (dolist (j set)
   (push (shrink j) result))
  (reverse result)))

(defun product (word1 word2)
 (shrink (concatenate 'string word1 word2)))

(defun make-table (set)
 (let ((maxlen 0) line linelen p (result (reverse set)))
  (dolist (i set)
   (setq maxlen (max (length i) maxlen)))
  (setq maxlen (+ 1 (* 2 maxlen)))
  (setq linelen (* maxlen (+ 1 (length set))))
  (setq line (make-string linelen :initial-element #\ ))
  (dotimes (i (length set))
   (insertInString line (elt set i) (* maxlen (+ i 1))))
  (print line)
  (dolist (row set)
    (setq line (make-string linelen :initial-element #\ ))
    (insertInString line row 0)
    (dotimes (i (length set))
     (setq p (product row (elt set i)))
     (setq result (adjoin p result :test #'string=))
     (insertInString line (product row (elt set i)) (* maxlen (+ i 1))))
    (print line))
 (reverse result)))

(defun insertInString (string word location)
 (dotimes (i (length word))
  (setf (elt string (+ location i)) (elt word i))))
  


(defun main ()
 (prt (cons " " a))
 (setq l (length a))
 (setq a (column a))
 (prt (pinch a)))

(defun column (set)
 (let (result)
  (dolist (j set)
   (setq result (concatenate 'list (row j '("a" "b" "A" "B")) result)))
  (reverse result)))

(defun prt (set)
 (let ((k 0))
  (dotimes (m l)
   (dolist (j set)
    (format t "~a " j))
   (terpri)
   (format t "~a " (elt set k))
   (setq k (+ k 1)))))



