### Определения

---

**Определение №1:** Пусть для некоторой точки $x_0$ задано $n+1$ узлов интерполяции $x_k=x_0+i \cdot h$, $k=0,...,n$ с шагом $h > 0$ и известны значения функции $f$ в этих точках. А так же определен порядок $m > 0$.

Тогда можно определить следующие понятия:
* Восходящая разность:
$$\Delta_h^1 (f, x) = f(x+h) - f(x)$$
$$\Delta_h^2 (f, x) = f(x+2h) - 2f(x+h) + f(x)$$
$$\Delta_h^m (f, x) = \sum_{i=0}^{\infty}{ (-1)^{m-i} \begin{pmatrix}m\\i\end{pmatrix} f(x+ih) }$$
* Низходящая разность:
$$\nabla_h^1 (f, x) = f(x) - f(x-h)$$
$$\nabla_h^2 (f, x) = f(x) - 2f(x-h) + f(x-2h)$$
$$\nabla_h^m (f, x) = \sum_{i=0}^{\infty}{ (-1)^{i} \begin{pmatrix}m\\i\end{pmatrix} f(x-ih) }$$
* Центральная разность:
$$\delta_h^1 (f, x) = f(x+\frac{h}{2}) - f(x-\frac{h}{2})$$
$$\delta_h^2 (f, x) = f(x+h) - 2f(x) + f(x-h)$$
$$\delta_h^m (f, x) = \sum_{i=0}^{\infty}{ (-1)^{i} \begin{pmatrix}m\\i\end{pmatrix} f\left(x+(\frac{m}{2}-i)h\right) }$$

где:
$$\begin{pmatrix}m\\i\end{pmatrix} = C^i_m = \frac{m!}{i!(m-i)!} = \frac{\Gamma(m+1)}{\Gamma(i+1)\Gamma(m-i+1)}$$

|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|m=0|   |   |   |   |   |   |  1|   |   |   |   |   |   |   |  
|m=1|   |   |   |   |   |  1|   | -1|   |   |   |   |   |   |   
|m=2|   |   |   |   |  1|   | -2|   |  1|   |   |   |   |   |   
|m=3|   |   |   |  1|   | -3|   |  3|   | -1|   |   |   |   |   
|m=4|   |   |  1|   | -4|   |  6|   | -4|   |  1|   |   |   |   
|m=5|   |  1|   | -5|   | 10|   |-10|   |  5|   | -1|   |   |
|m=6|  1|   | -6|   | 15|   |-20|   | 15|   | -6|   |  1|   |

---
**Утверждение №1:** 
$$\boxed{(-1)^iC_m^i = \frac{\Gamma(i-m)}{\Gamma(-m)\Gamma(i+1)}; i,m \in Z}$$
**Доказательство:** сделаем преобразования над $\frac{\Gamma(m+1)}{\Gamma(m-i+1)}$. 

Учтём, что:
$\Gamma(1-z)\Gamma(z) = \frac{\pi}{sin(\pi z)} \Rightarrow \Gamma(z) = \frac{\pi}{\Gamma(1-z)sin(\pi z)}$. Тогда:

$$\frac{\Gamma(m+1)}{\Gamma(m-i+1)} = \frac{m}{m-i} \cdot \frac{\Gamma(m)}{\Gamma(m-i)} = $$
$$ = \frac{m}{m-i} \cdot \frac{\pi}{\Gamma(1-m)sin(\pi m)} \cdot \frac{\Gamma(i-m+1)sin(\pi (m-i))}{\pi} = $$
$$ = \frac{m}{m-i} \cdot \frac{\Gamma(i-m+1)sin(\pi (m-i))}{\Gamma(1-m)sin(\pi m)} = $$
$$ = \frac{sin(\pi (m-i))}{sin(\pi m)} \cdot \frac{\Gamma(i-m)}{\Gamma(-m)} = (-1)^i\frac{\Gamma(i-m)}{\Gamma(-m)} \Rightarrow ч.т.д.$$

---
---

**Определение №2:** Пусть нам дана функция $f(x)$, которая определена справа(1)/слева(2) в некоторой окрестности точки $x$. Тогда:

1. <u>Правосторонней</u> производной называется предел:

$$f'_+(x) = \lim_{h \rightarrow 0}{\frac{f(x+h)-f(x)}{h}} = \lim_{h \rightarrow 0}{\frac{\Delta_h^1(f, x)}{h}}$$

2. <u>Левосторонней</u> производной называется предел:

$$f'_-(x) = \lim_{h \rightarrow 0}{\frac{f(x)-f(x-h)}{h}} = \lim_{h \rightarrow 0}{\frac{\nabla_h^1(f, x)}{h}}$$

---
---

**Определение №3:** Производной Грюнвальда-Летникова называется обобщение производной целых порядков на вещественные:

1. <u>Правосторонней</u> производной называется предел:

$$D_{b-}^\alpha f(x) = \lim_{h \rightarrow 0}{\frac{\Delta_{h}^\alpha(f, x)}{h^\alpha}} = \lim_{h \rightarrow 0}{\frac{(-1)^{\lceil \alpha \rceil}}{h^\alpha}}\sum_{i=0}^{\infty}{ (-1)^{i} \begin{pmatrix}\alpha\\i\end{pmatrix} f(x+ih) } $$

2. <u>Левосторонней</u> производной называется предел:

$$D_{a+}^\alpha f(x) = \lim_{h \rightarrow 0}{\frac{\nabla_h^\alpha(f, x)}{h^\alpha}} = \lim_{h \rightarrow 0}{\frac{1}{h^\alpha}}\sum_{i=0}^{\infty}{ (-1)^{i} \begin{pmatrix}\alpha\\i\end{pmatrix} f(x-ih) }$$

$D_{b-}^n f(x) = f^n_+(x); n \in Z $

$D_{a+}^n f(x) = f^n_-(x); n \in Z $

**Вывод:** Введем оператор $E^t: f(x) \rightarrow f(x+t)$. Обозначим $I=E^0$. Тогда:

$$\frac{d}{dx}f(x) \approx \frac{f(x) - f(x - h)}{h} = \frac{(I-E^{-h})f(x)}{h}$$

---
**Утверждение №2:**

$$\boxed{(E^{-h})^n = E^{-nh}}$$

**Доказательство:** По индукции:

1. $(E^{-h})^1f(x) = f(x-h) = E^{-1 \cdot h}f(x)$
2. $(E^{-h})^2f(x) = (E^{-h})(E^{-h})f(x) = (E^{-h})f(x-h) = f(x-2h) = E^{-2h}$
3. $(E^{-h})^{k+1}f(x) = (E^{-h})(E^{-h})^{k}f(x) = (E^{-h})f(x-kh) = f(x-(k+1)h) = E^{-(k+1)h}f(x)$

---

$$\frac{(I-E^{-h})^nf(x)}{h^n} = \frac{1}{h^n} \sum_{i=0}^{n}{(-1)^{i}\begin{pmatrix}n\\i\end{pmatrix}I^{n-i}(E^{-h})^{i}f(x)} =$$
$$= \frac{1}{h^n} \sum_{i=0}^{n}{(-1)^{i}\begin{pmatrix}n\\i\end{pmatrix}f(x-ih)} \approx \frac{d^n}{dx^n}f(x)$$

Из рядов Тейлора имеем:

$$(1+x)^\alpha = \sum_{k=0}^{\infty}{\begin{pmatrix}n\\k\end{pmatrix}x^k}; |x| \leq 1 (\text{при } \alpha \geq 0)$$

А так как:

$$\lim_{h \rightarrow 0}||E^{-h}|| = \lim_{h \rightarrow 0}\left(\sup_{||f(x)||=1}||f(x-h)|| \right)\leq 1$$

Можно записать для $\alpha>0$; $h \rightarrow 0$:

$$ \frac{(I-E^{-h})^\alpha f(x)}{h^\alpha} = \frac{1}{h^\alpha} \sum_{i=0}^{\infty}{(-1)^{i}\begin{pmatrix}\alpha\\i\end{pmatrix}f(x-ih)} \approx \frac{d^\alpha}{dx^\alpha}f(x) $$

---

**Интегралы:**

|$f(x)$|$$\int_a^b{\frac{f(s)ds}{(x-s)^\alpha}}$$|
|-|-|
|$A$|$$\frac{A}{1-\alpha}\left[ (x-a)^{1-\alpha}-(x-b)^{1-\alpha} \right]$$|
|$Ax+B$|$$\frac{A}{2-\alpha}\left[ (x-b)^{2-\alpha}-(x-a)^{2-\alpha} \right]+\frac{Ax+B}{1-\alpha} \left[ (x-a)^{1-\alpha}-(x-b)^{1-\alpha} \right]$$|
|$(Ax+B)^n$|$$\sum_{i=0}^{n}{\sum_{j=0}^{n-i}{  (-1)^j A^{n-i}B^i\begin{pmatrix}n\\i\end{pmatrix}\begin{pmatrix}n-i\\j\end{pmatrix} \frac{x^{n-i-j}}{j+1-i} \left[{(x-a)^{j+1-i}-(x-b)^{j+1-i}}\right]   }}$$|



---