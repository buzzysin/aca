#ifndef SUBJECT_ANON_H
#define SUBJECT_ANON_H

#include "Subject.h"

template <typename T>
class SubjectAnon : public Subject<T>
{
  OBJECT(SubjectAnon<T>)

protected:
  Observer<T> *_destination;
  LiftedObservable<T> *_source;

public:
  SubjectAnon(Observer<T> *destination, LiftedObservable<T> *source);

  virtual ~SubjectAnon() = default;

public:
  void next(const T &value) override;
  void error(const std::exception &error) override;
  void complete() override;

protected:
  Subscription *_subscribe(Subscriber<T> *subscriber) override;
};

#include "SubjectAnon.tpp"

#endif