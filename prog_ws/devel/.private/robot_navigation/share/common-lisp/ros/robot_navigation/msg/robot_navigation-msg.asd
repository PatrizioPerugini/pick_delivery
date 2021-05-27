
(cl:in-package :asdf)

(defsystem "robot_navigation-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "new_goal" :depends-on ("_package_new_goal"))
    (:file "_package_new_goal" :depends-on ("_package"))
  ))