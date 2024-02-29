# FDESolver
Solver of anomalous diffusion differential equation.

Task:

$$
\frac{\partial^\gamma}{\partial t^\gamma}u(x, t) = 
D(x)\left( \frac{(1+\beta)}{2}\frac{\partial _+^\alpha}{\partial x^\alpha} + \frac{(1-\beta)}{2}\frac{\partial _-^\alpha}{\partial x^\alpha} \right)u(x, t) + V(x)\frac{\partial}{\partial x}u(x, t) + f(x, t)
$$

$$
u(x, 0) = \psi(x) 
$$

$$
u(L, t) = \phi_L(t) 
$$

$$
u(R, t) = \phi_R(t) 
$$

$$
\alpha \in (1; 2); \gamma \in (0; 1); \beta \in [-1; 1]
$$
