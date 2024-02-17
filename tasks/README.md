### Определения

---

**Определение №1:** Пусть для некоторой точки $x_0$ задано $n+1$ узлов интерполяции $x_k=x_0+i \cdot h$, $k=0,...,n$ с шагом $h > 0$ и известны значения функции $f$ в этих точках. А так же определен порядок $m > 0$.

Тогда можно определить следующие понятия:
* Восходящая разность:
$$\Delta_h^1 (f, x) = f(x+h) - f(x)$$
$$\Delta_h^2 (f, x) = f(x+2h) - 2f(x+h) + f(x)$$
$$\Delta_h^m (f, x) = \sum_{i=0}^{\infty}{ (-1)^{m-i} {m \choose i} f(x+ih) }$$
* Низходящая разность:
$$\nabla_h^1 (f, x) = f(x) - f(x-h)$$
$$\nabla_h^2 (f, x) = f(x) - 2f(x-h) + f(x-2h)$$
$$\nabla_h^m (f, x) = \sum_{i=0}^{\infty}{ (-1)^{i} {m \choose i} f(x-ih) }$$
* Центральная разность:
$$\delta_h^1 (f, x) = f(x+\frac{h}{2}) - f(x-\frac{h}{2})$$
$$\delta_h^2 (f, x) = f(x+h) - 2f(x) + f(x-h)$$
$$\delta_h^m (f, x) = \sum_{i=0}^{\infty}{ (-1)^{i} {m \choose i} f\left(x+(\frac{m}{2}-i)h\right) }$$

где:
$${m \choose i} = C^i_m = \frac{m!}{i!(m-i)!} = \frac{\Gamma(m+1)}{\Gamma(i+1)\Gamma(m-i+1)}$$

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

$$D_{b-}^\alpha f(x) = \lim_{h \rightarrow 0}{\frac{\Delta_{h}^\alpha(f, x)}{h^\alpha}} = \lim_{h \rightarrow 0}{\frac{(-1)^{\lceil \alpha \rceil}}{h^\alpha}}\sum_{i=0}^{\infty}{ (-1)^{i} {\alpha \choose i} f(x+ih) } $$

2. <u>Левосторонней</u> производной называется предел:

$$D_{a+}^\alpha f(x) = \lim_{h \rightarrow 0}{\frac{\nabla_h^\alpha(f, x)}{h^\alpha}} = \lim_{h \rightarrow 0}{\frac{1}{h^\alpha}}\sum_{i=0}^{\infty}{ (-1)^{i} {\alpha \choose i} f(x-ih) }$$

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

$$\frac{(I-E^{-h})^nf(x)}{h^n} = \frac{1}{h^n} \sum_{i=0}^{n}{(-1)^{i} {n \choose i} I^{n-i}(E^{-h})^{i}f(x)} =$$
$$= \frac{1}{h^n} \sum_{i=0}^{n}{(-1)^{i}{n \choose i}f(x-ih)} \approx \frac{d^n}{dx^n}f(x)$$

Из рядов Тейлора имеем:

$$(1+x)^\alpha = \sum_{k=0}^{\infty}{{n \choose k}x^k}; |x| \leq 1 (\text{при } \alpha \geq 0)$$

А так как:

$$\lim_{h \rightarrow 0}||E^{-h}|| = \lim_{h \rightarrow 0}\left(\sup_{||f(x)||=1}||f(x-h)|| \right)\leq 1$$

Можно записать для $\alpha>0$; $h \rightarrow 0$:

$$ \frac{(I-E^{-h})^\alpha f(x)}{h^\alpha} = \frac{1}{h^\alpha} \sum_{i=0}^{\infty}{(-1)^{i}{\alpha \choose i}f(x-ih)} \approx \frac{d^\alpha}{dx^\alpha}f(x) $$

---

**Интегралы:**

Сокращения и свойства:

1. $\begin{aligned} K(x, a, b, \alpha) = \frac{1}{\alpha}\left[ (x-a)^{\alpha}-(x-b)^{\alpha} \right]                   \end{aligned}$
2. $\begin{aligned} \frac{d^m}{dx^m} K(x, a, b, \alpha) = \frac{\Gamma(\alpha)}{\Gamma(\alpha-m)} K(x, a, b, \alpha-m)  \end{aligned}$

Таблица 1 - таблица интегралов и производных
|№|$f(x)$|$$\int_a^b{\frac{f(s)ds}{(x-s)^\alpha}}$$|$$D^\alpha_{a+,x}f(x)=\frac{1}{\Gamma(m-\alpha)}\frac{d^m}{dx^m}\int_a^x{\frac{f(s)ds}{(x-s)^{1+\alpha-m}}}$$|
|-|-|-|-|
|1.|$A$|$\begin{aligned} A \cdot K(x, a, b, 1-\alpha) \end{aligned}$|$\begin{aligned} \frac{A}{\Gamma(1-\alpha)}(x-a)^{-\alpha} \end{aligned}$|
|2.|$Ax+B$|$\begin{aligned}A \cdot K(x, b, a, 2-\alpha) + (Ax+B) K(x, a, b, 1-\alpha)\end{aligned}$||
|3.|$Ax^n$|$\begin{aligned} A \cdot \sum_{i=0}^{n}{(-1)^i {n \choose i} x^{n-i} K(x, a, b, i+1-\alpha) } \end{aligned}$||
|4.|$Ax^\beta$|$\begin{aligned} Ax^{1+\beta-\alpha} \int_{1-\frac{b}{x}}^{1-\frac{a}{x}}{p^{-\alpha} (1-p)^\beta dp} \end{aligned}$ | $\begin{aligned} \frac{A}{\Gamma(m-\alpha)} \left[ \frac{\Gamma(m+\beta-\alpha+1)}{\Gamma(\beta-\alpha+1)} x^{\beta-\alpha} \Beta_{1-\frac{a}{x}}(m-\alpha, \beta+1) + \sum_{i=1}^{m} {{m \choose i} \left(  \frac{\Gamma(m+\beta-\alpha+1)}{\Gamma(\beta-\alpha+i+1)} x^{\beta-\alpha+i}\right) \frac{d^{i-1}}{dx^{i-1}}\left( a^{\beta+1} \frac{(x-a)^{m-\alpha-1}}{x^{m+\beta-\alpha+1}} \right) } \right] \end{aligned}$ <br> При $a=0$ всё упрощается: <br> $\begin{aligned} \frac{\Gamma(\beta+1)}{\Gamma(\beta-\alpha+1)} A x^{\beta-\alpha} \end{aligned}$ |
|5.|$(Ax+B)^n$|$\begin{aligned}\sum_{i=0}^{n}{\sum_{j=0}^{n-i}{  (-1)^j A^{n-i}B^i{n \choose i}\begin{pmatrix}n-i\\j\end{pmatrix} x^{n-i-j} K(x, a, b, j+1-\alpha)   }}\end{aligned}$||

**Вспомогательные равенства:**

$(1) 
\begin{aligned} 
    \int_a^b{\frac{f(s)ds}{(x-s)^\alpha}} = 
    \begin{bmatrix}p=x-s\\dp=-ds\\p(b)=x-b\\p(a)=x-a\end{bmatrix} = 
    \int_{x-b}^{x-a}{\frac{f(x-p)dp}{p^\alpha}} 
\end{aligned}
$

$(2) 
\begin{aligned} 
    \int_a^b{\frac{f(s)ds}{(x-s)^\alpha}} = 
    \begin{bmatrix}p=1-\frac{s}{x}\\s=x-xp\\ds=-xdp\\p(b)=1-\frac{b}{x}\\p(a)=1-\frac{a}{x}\end{bmatrix} = 
    \int_{1-\frac{b}{x}}^{1-\frac{a}{x}}{\frac{x^{1-\alpha} \cdot f(x-xp)dp}{p^\alpha}} 
\end{aligned}
$

**Интеграл №1:**

$
\begin{aligned} 
    \int_a^b{\frac{Ads}{(x-s)^\alpha}} \overset{(1)}{=}
    A\int_{x-b}^{x-a}{p^{-\alpha}dp} = 
    A\left.\frac{p^{1-\alpha}}{1-\alpha} \right\vert_{x-b}^{x-a} = 
    A\frac{\left[(x-a)^{1-\alpha}-(x-b)^{1-\alpha}\right]}{1-\alpha} = 
    A \cdot K(x, a, b, 1-\alpha) 
\end{aligned}
$

**Интеграл №2:**

$
\begin{aligned} 
    \int_a^b{\frac{sds}{(x-s)^\alpha}} \overset{(1)}{=} 
    \int_{x-b}^{x-a}{(x-p)p^{-\alpha}dp} = 
    \left.\left(x\frac{p^{1-\alpha}}{1-\alpha} - \frac{p^{2-\alpha}}{2-\alpha} \right)\right\vert_{x-b}^{x-a} = 
    x \cdot K(x, a, b, 1-\alpha) - K(x, a, b, 2-\alpha) = 
    x \cdot K(x, a, b, 1-\alpha) + K(x, b, a, 2-\alpha) 
\end{aligned}
$

**Интеграл №3:**

$
\begin{aligned} 
    \int_a^b{\frac{s^\beta ds}{(x-s)^\alpha}} \overset{(2)}{=} 
    \int_{1-\frac{b}{x}}^{1-\frac{a}{x}}{\frac{x^{1-\alpha} \cdot x^\beta (1-p)^\beta dp}{p^\alpha}} =
    x^{1+\beta-\alpha} \int_{1-\frac{b}{x}}^{1-\frac{a}{x}}{p^{-\alpha} (1-p)^\beta dp}
\end{aligned}
$

Пусть $b=x$:

$
\begin{aligned}
    \int_{0}^{1-\frac{a}{x}}{p^{-\alpha} (1-p)^\beta dp} = 
    B_{1-\frac{a}{x}}(1-\alpha, \beta+1) = 
    B(1-\alpha, \beta+1) \left[ 1-I_{\frac{a}{x}}(\beta+1,1-\alpha) \right]
\end{aligned}
$

---