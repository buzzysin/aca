#ifndef COMPONENT_H
#define COMPONENT_H

template <typename Inputs, typename Outputs> class Component {
protected:
  Inputs _inputs;
  Outputs _outputs;

public:
  Component(const Inputs &inputs, const Outputs &outputs);
  virtual ~Component();

public:
  Inputs &inputs() const;
  Outputs &outputs() const;
};

#endif // COMPONENT_H