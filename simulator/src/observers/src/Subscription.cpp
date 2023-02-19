#include "Subscription.h"

#include <iostream>

Subscription::Subscription(CanTeardown *teardown) {

  tkDebug() << "Subscription::Subscription()";

  if (teardown == nullptr)
    _teardown = CanTeardown::toFunction(this);
}

Subscription::~Subscription() {

  tkDebug() << "Subscription::~Subscription()";

  teardown();
}

bool Subscription::closed() const {

  tkDebug() << "Subscription::closed() = " << _closed;

  return _closed;
}

void Subscription::teardown() {

  tkDebug() << "Subscription::teardown()";

  unsubscribe();
}

void Subscription::unsubscribe() {
  tkDebug() << "Subscription::unsubscribe()";

  auto errors = std::vector<std::exception>();

  if (closed())
    return;

  _closed = true;

  for (size_t i = 0; i < _parents.size(); i++) {
    _parents.front()->remove(this);
  }

  if (_teardown) {
    try {
      _teardown();
    } catch (const std::exception &error) {
      errors.push_back(error);
    }
  }

  for (size_t i = 0; i < _teardowns.size(); i++) {
    auto teardown = _teardowns.front();

    // Remove the teardown from the list
    _teardowns.erase(_teardowns.begin());

    try {
      teardown->teardown();
    } catch (const std::exception &error) {
      errors.push_back(error);
    }
  }

  if (errors.size() > 0) {
    // For now, just throw the first error
    throw errors.front();
  }
}

void Subscription::add(Subscription *subscription) {

  tkDebug() << "Subscription::add(Subscription *)";

  if (subscription == nullptr)
    return;

  if (subscription == this)
    return;

  if (closed()) {
    subscription->teardown();
    return;
  }

  if (subscription->closed() || subscription->_hasParent(this))
    return;

  subscription->_addParent(this);

  _teardowns.push_back(subscription);
}

void Subscription::add(CanUnsubscribe *unsubscribe) {

  tkDebug() << "Subscription::add(CanSunbscribe *)";

  if (unsubscribe == nullptr)
    return;

  if (closed()) {
    unsubscribe->unsubscribe();
    return;
  }

  _teardowns.push_back(unsubscribe);
}

void Subscription::add(const std::function<void(void)> teardown) {

  tkDebug() << "Subscription::add()";

  if (closed()) {
    teardown();
    return;
  }

  _teardowns.push_back(new CanTeardown(teardown));
}

void Subscription::remove(Subscription *subscription) {
  tkDebug() << "Subscription::remove()" << *subscription;

  for (size_t i = 0; i < _teardowns.size(); i++) {
    auto teardown = _teardowns.front();

    // pop the first element <vector> style (NOT POP_FRONT!)
    _teardowns.erase(_teardowns.begin());

    if (teardown == subscription) {
      subscription->_removeParent(this);
      continue;
    }

    _teardowns.push_back(teardown);
  }

  subscription->_removeParent(this);
}

void Subscription::remove(CanUnsubscribe *unsubscribe) {
  tkDebug() << "Subscription::remove()";

  for (auto it = _teardowns.end(); it != _teardowns.begin(); it++)
    if (*it == unsubscribe) {
      _teardowns.erase(it);
      break;
    }
}

void Subscription::remove(const std::function<void(void)> teardown) {

  tkDebug() << "Subscription::remove()";

  for (auto it = _teardowns.end(); it != _teardowns.begin(); it--)
    if ((*it)->reference() == &teardown) {
      _teardowns.erase(it);
      break;
    }
}

bool Subscription::_hasParent(Subscription *parent) {
  tkDebug().nolines() << "Subscription::_hasParent() : checking if " << *this
                      << " has the parent " << *parent << "\n";

  for (auto &p : _parents) {
    if (p == parent) {
      tkDebug().nolines() << "Subscription::_hasParent() : " << *this
                          << " has the parent " << *parent << "\n";

      return true;
    }
  }

  tkDebug().nolines() << "Subscription::_hasParent() : " << *this
                      << " does not have the parent " << *parent << "\n";

  return false;
}

void Subscription::_addParent(Subscription *parent) {
  tkDebug().nolines() << "Subscription::_addParent() : adding parent "
                      << *parent << " to " << *this << " with "
                      << _parents.size() << " parents\n";

  _parents.push_back(parent);
}

void Subscription::_removeParent(Subscription *parent) {
  tkDebug() << "Subscription::_removeParent()";

  for (auto it = _parents.end(); it != _parents.begin(); it--)
    if (*it == parent) {
      _parents.erase(it);
      return;
    }
}