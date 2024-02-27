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

$$\frac{\Gamma(m+1)}{\Gamma(m-i+1)} = \frac{m}{m-i} \cdot \frac{\Gamma(m)}{\Gamma(m-i)} =$$

$$= \frac{m}{m-i} \cdot \frac{\pi}{\Gamma(1-m)sin(\pi m)} \cdot \frac{\Gamma(i-m+1)sin(\pi (m-i))}{\pi} =$$

$$= \frac{m}{m-i} \cdot \frac{\Gamma(i-m+1)sin(\pi (m-i))}{\Gamma(1-m)sin(\pi m)} =$$

$$= \frac{sin(\pi (m-i))}{sin(\pi m)} \cdot \frac{\Gamma(i-m)}{\Gamma(-m)} = (-1)^i\frac{\Gamma(i-m)}{\Gamma(-m)} \Rightarrow ч.т.д.$$

---
---

**Определение №2:** Пусть нам дана функция $f(x)$, которая определена справа(1)/слева(2) в некоторой окрестности точки $x$. Тогда:

1. <u>Правосторонней</u> производной называется предел:

$$\frac{d^+}{dx}f(x) = \lim_{h \rightarrow 0}{\frac{f(x+h)-f(x)}{h}} = \lim_{h \rightarrow 0}{\frac{\Delta_h^1(f, x)}{h}}$$

2. <u>Левосторонней</u> производной называется предел:

$$\frac{d^-f(x)}{dx} = \lim_{h \rightarrow 0}{\frac{f(x)-f(x-h)}{h}} = \lim_{h \rightarrow 0}{\frac{\nabla_h^1(f, x)}{h}}$$

---
---
**Определение №3:** Производной Римана-Лиувиля называется следующие выражения:

1. <u>Правосторонней</u> производной называется выражение:

$$D_{b-}^\alpha f(x) = \frac{(-1)^m}{\Gamma(m-\alpha)}\frac{d^m}{dx^m} \int\limits_{x}^{b}{\frac{f(s)}{(s-x)^{1+\alpha-m}}ds}$$

2. <u>Левосторонней</u> производной называется выражение:

$$D_{a+}^\alpha f(x) = \frac{1}{\Gamma(m-\alpha)}\frac{d^m}{dx^m} \int\limits_{a}^{x}{\frac{f(s)}{(x-s)^{1+\alpha-m}}ds}$$

где $m = \lceil a \rceil$.

---

**Определение №4:** Производной Грюнвальда-Летникова называются следующие пределы:

1. <u>Правосторонней</u> производной называется предел:

$$D_{b-}^\alpha f(x) = \lim_{h \rightarrow 0}{\frac{\Delta_{h}^\alpha(f, x)}{h^\alpha}} = \lim_{h \rightarrow 0}{\frac{(-1)^{\lceil \alpha \rceil}}{h^\alpha}}\sum_{i=0}^{\infty}{ (-1)^{i} {\alpha \choose i} f(x+ih) }$$

2. <u>Левосторонней</u> производной называется предел:

$$D_{a+}^\alpha f(x) = \lim_{h \rightarrow 0}{\frac{\nabla_h^\alpha(f, x)}{h^\alpha}} = \lim_{h \rightarrow 0}{\frac{1}{h^\alpha}}\sum_{i=0}^{\infty}{ (-1)^{i} {\alpha \choose i} f(x-ih) }$$

$D_{b-}^n f(x) = f^n_+(x); n \in Z$

$D_{a+}^n f(x) = f^n_-(x); n \in Z$

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

$$\frac{(I-E^{-h})^\alpha f(x)}{h^\alpha} = \frac{1}{h^\alpha} \sum_{i=0}^{\infty}{(-1)^{i}{\alpha \choose i}f(x-ih)} \approx \frac{d^\alpha}{dx^\alpha}f(x)$$

---

**Интегралы:**

Сокращения и свойства:

1. $K(x, a, b, \alpha) = \frac{1}{\alpha}\left[ (x-a)^{\alpha}-(x-b)^{\alpha} \right]$
2. $\frac{d^m}{dx^m} K(x, a, b, \alpha) = \frac{\Gamma(\alpha)}{\Gamma(\alpha-m)} K(x, a, b, \alpha-m)$

Таблица 1 - таблица m-кратного дифференциала
|$№$|$$f(x)$$|$$\frac{d^m}{dx^m}f(x)$$|
|-|-|-|
|1.|$$x^n$$|$$\frac{\Gamma(n+1)}{\Gamma(n-m+1)}x^{n-m}$$|
|2.|$$f(x)g(x)$$|$$\sum_{i=0}^{m}{ {m \choose i} \frac{d^{m-i}}{dx^{m-i}}f(x) \frac{d^i}{dx^i}g(x) }$$|

Таблица 2 - таблица производных
|$№$|$f(x)$|$$D^\alpha_{a+,x}f(x)=\frac{1}{\Gamma(m-\alpha)}\frac{d^m}{dx^m}\int_a^x{\frac{f(s)ds}{(x-s)^{1+\alpha-m}}}$$|
|-|-|-|
|1.|$A$ | $$\frac{A}{\Gamma(1 - \alpha)}(x-a)^{-\alpha}$$ |
|2.|$A(x-a)^\beta$ | $$\frac{A \cdot \Gamma(\beta + 1)}{\Gamma(\beta + 1 - \alpha)}(x-a)^{\beta - \alpha}$$ |
|3.|$(x-a)^{\beta}ln(x-a)$ | $$\frac{\Gamma(\beta + 1)}{\Gamma(\beta + 1 - \alpha)}(x-a)^{\beta - \alpha} \left[ ln(x-a) + \psi^{(0)}(\beta + 1) - \psi^{(0)}(\beta + 1 - \alpha) \right]$$ |

**Вспомогательные равенства:**

$$(1)
\int_a^b{\frac{f(s)ds}{(x-s)^\alpha}} = 
\begin{bmatrix}p=x-s \\\\ dp=-ds \\\\ p(b)=x-b \\\\ p(a)=x-a\end{bmatrix} = 
\int_{x-b}^{x-a}{\frac{f(x-p)dp}{p^\alpha}} 
$$

$$(2)
\int_a^b{\frac{f(s)ds}{(x-s)^\alpha}} = 
\begin{bmatrix}p=1-\frac{s}{x} \\\\ s=x-xp \\\\ ds=-xdp \\\\ p(b)=1-\frac{b}{x} \\\\ p(a)=1-\frac{a}{x}\end{bmatrix} = 
\int_{1-\frac{b}{x}}^{1-\frac{a}{x}}{\frac{x^{1-\alpha} \cdot f(x-xp)dp}{p^\alpha}} 
$$

**Интеграл №1:**

$$
\int_a^b{\frac{Ads}{(x-s)^\alpha}} \overset{(1)}{=}
A\int_{x-b}^{x-a}{p^{-\alpha}dp} = 
A\left.\frac{p^{1-\alpha}}{1-\alpha} \right\vert_{x-b}^{x-a} = 
A\frac{\left[(x-a)^{1-\alpha}-(x-b)^{1-\alpha}\right]}{1-\alpha} = 
A \cdot K(x, a, b, 1-\alpha) 
$$

**Интеграл №2:**

$$
\int_a^b{\frac{sds}{(x-s)^\alpha}} \overset{(1)}{=} 
\int_{x-b}^{x-a}{(x-p)p^{-\alpha}dp} = 
\left.\left(x\frac{p^{1-\alpha}}{1-\alpha} - \frac{p^{2-\alpha}}{2-\alpha} \right)\right\vert_{x-b}^{x-a} = 
x \cdot K(x, a, b, 1-\alpha) - K(x, a, b, 2-\alpha) = 
x \cdot K(x, a, b, 1-\alpha) + K(x, b, a, 2-\alpha) 
$$

**Интеграл №3:**

$$
\int_a^b{\frac{s^\beta ds}{(x-s)^\alpha}} \overset{(2)}{=} 
\int_{1-\frac{b}{x}}^{1-\frac{a}{x}}{\frac{x^{1-\alpha} \cdot x^\beta (1-p)^\beta dp}{p^\alpha}} =
x^{1+\beta-\alpha} \int_{1-\frac{b}{x}}^{1-\frac{a}{x}}{p^{-\alpha} (1-p)^\beta dp}
$$

Пусть $b=x$:

$$
\int_{0}^{1-\frac{a}{x}}{p^{-\alpha} (1-p)^\beta dp} = 
B_{1-\frac{a}{x}}(1-\alpha, \beta+1) = 
B(1-\alpha, \beta+1) \left[ 1-I_{\frac{a}{x}}(\beta+1,1-\alpha) \right]
$$

---

# Численные методы

Задача:

$$
\begin{cases}
\frac{\partial^\gamma}{\partial t^\gamma}u(x, t) = D\frac{\partial^\alpha}{\partial x^\alpha}u(x, t) + V\frac{\partial}{\partial x}u(x, t) + f(x, t) \\
u(x, 0) = \psi(x) \\ 
\alpha \in (1; 2) \\
\gamma \in (0; 1)
\end{cases}
$$

Комбинированное представление производной по пространству:

$$
\frac{\partial^\alpha}{\partial x^\alpha}u(x, t) = \frac{(1+\beta)}{2}\frac{\partial_+^\alpha}{\partial x^\alpha}u(x, t) + \frac{(1-\beta)}{2}\frac{\partial_-^\alpha}{\partial x^\alpha}u(x, t);\text{\ \ \ } \beta \in [-1;1]
$$

Вывод через определение Римана-Лиувилля:

$$\left. \frac{\partial_+^\alpha}{\partial x^\alpha}u(x, t)\right|_ {i} = \frac{1}{\Gamma(2-\alpha)}\frac{d^2}{d x^2} \int\limits_{a}^{x}{\frac{u(s, t)}{(x-s)^{\alpha-1}}ds} = \frac{1}{\Gamma(2-\alpha)} \frac{{}_ +F_{i+\frac{1}{2}}^k - {}_ +F_{i-\frac{1}{2}}^k}{h}$$

$${}_ +F_{i+\frac{1}{2}}^k = \frac{d}{dx_{i+\frac{1}{2}}} \int\limits_{a}^{x_{i+\frac{1}{2}}}{\frac{u(s, t)}{(x_{i+\frac{1}{2}}-s)^{\alpha-1}}ds} = \frac{d}{dx_{i+\frac{1}{2}}} 
\left[ 
    \sum_{j=0}^{i-1}{\int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(x_{i+\frac{1}{2}}-s)^{\alpha-1}}ds}} + \int\limits_{x_i}^{x_{i+\frac{1}{2}}}{\frac{A_is+B_i}{(x_{i+\frac{1}{2}}-s)^{\alpha-1}}ds} 
\right]
$$

$${}_ +F_{i-\frac{1}{2}}^k = \frac{d}{dx_{i-\frac{1}{2}}} \int\limits_{a}^{x_{i-\frac{1}{2}}}{\frac{u(s, t)}{(x_{i-\frac{1}{2}}-s)^{\alpha-1}}ds} = \frac{d}{dx_{i-\frac{1}{2}}} 
\left[ 
    \sum_{j=0}^{i-2}{\int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(x_{i-\frac{1}{2}}-s)^{\alpha-1}}ds}} + \int\limits_{x_{i-1}}^{x_{i-\frac{1}{2}}}{\frac{A_{i-1}s+B_{i-1}}{(x_{i-\frac{1}{2}}-s)^{\alpha-1}}ds} 
\right]
$$

$A_j = \frac{u_{j+1}^k - u_j^k}{h} $

---

$$
\frac{d}{dx_{p}} \int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(x_{p}-s)^{\alpha-1}}ds} \overset{(1)}{=} 
\frac{d}{dx_{p}} \int\limits_{h(p-j-1)}^{h(p-j)}{\frac{A_j(x_p - t)+B_j}{t^{\alpha-1}}dt} =
\frac{d}{dx_{p}} \int\limits_{h(p-j-1)}^{h(p-j)}{((A_jx_p+B_j)t^{1-\alpha} - A_jt^{2-\alpha})dt} =$$
$$=
\frac{d}{dx_{p}} \left. \left[ {(A_jx_p+B_j)\frac{t^{2-\alpha}}{2-\alpha} - A_j\frac{t^{3-\alpha}}{3-\alpha}} \right] \right|_{h(p-j-1)}^{h(p-j)} =
A_j\frac{h^{2-\alpha}}{2-\alpha}\left[ (p-j)^{2-\alpha} - (p-j-1)^{2-\alpha} \right]
$$

---

Получаем следующие потоки:

$${}_ +F_{i+\frac{1}{2}}^k =
\sum_{j=0}^{i-1}{\left[ A_j \frac{h^{2-\alpha}}{2-\alpha}\left[ (i-j+\frac{1}{2})^{2-\alpha} - (i-j-\frac{1}{2})^{2-\alpha} \right] \right]} + A_i\frac{h^{2-\alpha}}{2-\alpha}\frac{1}{2^{2-\alpha}}
$$


$${}_ +F_{i-\frac{1}{2}}^k =
\sum_{j=0}^{i-2}{\left[ A_j \frac{h^{2-\alpha}}{2-\alpha}\left[ (i-j-\frac{1}{2})^{2-\alpha} - (i-j-\frac{3}{2})^{2-\alpha} \right] \right]} + A_{i-1}\frac{h^{2-\alpha}}{2-\alpha}\frac{1}{2^{2-\alpha}} = 
$$

Выносим общие множители:

$${}_ +F_{i+\frac{1}{2}}^k =
\frac{h^{2-\alpha}}{(2-\alpha)2^{2-\alpha}} \left( \sum_{j=0}^{i-2}{\left[ A_j \left[ (2(i-j)+1)^{2-\alpha} - (2(i-j)-1)^{2-\alpha} \right] \right]} + A_i + (3^{2-\alpha}-1)A_{i-1}\right)
$$


$${}_ +F_{i-\frac{1}{2}}^k =
\frac{h^{2-\alpha}}{(2-\alpha)2^{2-\alpha}} \left( \sum_{j=0}^{i-2}{\left[ A_j \left[ (2(i-j)-1)^{2-\alpha} - (2(i-j)-3)^{2-\alpha} \right] \right]} + A_{i-1} \right)
$$

В промежуточном виде имеем:

$$\left. \frac{\partial^\alpha}{\partial x^\alpha}u(x, t) \right|_{i} = 
\frac{h^{1-\alpha}}{\Gamma(3-\alpha)2^{2-\alpha}} \cdot \left( \sum_{j=0}^{i-2}{\left[ A_j \left[ (2(i-j)+1)^{2-\alpha} -2(2(i-j)-1)^{2-\alpha} + (2(i-j)-3)^{2-\alpha} \right] \right]} + A_{i} + (3^{2-\alpha}-2)A_{i-1} \right)
$$

Введём следующие обозначения:

$$G_0 = \frac{1}{\Gamma(3-\alpha)2^{2-\alpha}}$$
$$G_1 = G_0 \cdot (3^{2-\alpha} - 2)$$
$$G_k = G_0 \cdot ((2k+1)^{2-\alpha}-2(2k-1)^{2-\alpha}+(2k-3)^{2-\alpha}); k \geq 2$$

Получаем:

$$\left. \frac{\partial^\alpha}{\partial x^\alpha}u(x, t) \right|_{i} = 
\frac{1}{h^{\alpha - 1}} \cdot \left( \sum_{j=0}^{i-2}(A_j G_{i-j}) + G_0A_{i} + G_1A_{i-1} \right) = 
\frac{1}{h^{\alpha}} \cdot \left( \sum_{j=0}^{i-2}((u_{j+1}^k-u_j^k) G_{i-j}) + G_0(u_{i+1}^k-u_i^k) + G_1(u_{i}^k-u_{i-1}^k) \right) = \\
\frac{1}{h^{\alpha}} \cdot \left( \sum_{j=1}^{i-1}u_{j}^k G_{i-j+1} - \sum_{j=0}^{i-2}u_j^k G_{i-j} + G_0(u_{i+1}^k-u_i^k) + G_1(u_{i}^k-u_{i-1}^k) \right) = \\
\frac{1}{h^{\alpha}} \cdot \left( \sum_{j=1}^{i-2}u_{j}^k (G_{i-j+1} - G_{i-j}) + G_{2} u^k_{i-1} - G_{i} u^k_{0} + G_0(u_{i+1}^k-u_i^k) + G_1(u_{i}^k-u_{i-1}^k) \right)
$$

Итого имеем:

$$\left. \frac{\partial _+^\alpha}{\partial x^\alpha}u(x, t) \right|_{i} =
\frac{1}{h^{\alpha}} \cdot \left(-G_{i} u^k_{0} + \sum_{j=1}^{i-2}u_{j}^k (G_{i-j+1} - G_{i-j}) + (G_2-G_1) u^k_{i-1} + (G_1-G_0) u_{i}^k + G_0 u_{i+1}^k \right)
$$

Тогда матрица для левой производной будет иметь следующий вид:

$$ L =
\begin{pmatrix}
G_1-G_0 & G_0 & 0 & 0 & \dots & 0 & 0 \\
-G_1 & G_1-G_0 & G_0 & 0 & \dots & 0 & 0 \\
-G_2 & G_2-G_1 & G_1-G_0 & G_0 & \dots & 0 & 0 \\
-G_3 & G_3-G_2 & G_2-G_1 & G_1-G_0 & \dots & 0 & 0 \\
\vdots & \vdots & \vdots & \vdots & \ddots & \vdots & \vdots \\
-G_{n-1} & G_{n-1}-G_{n-2} & G_{n-2}-G_{n-3} & G_{n-3}-G_{n-4} & \dots & G_1-G_0 & G_0\\
-G_n & G_n-G_{n-1} & G_{n-1}-G_{n-2} & G_{n-2}-G_{n-3} & \dots & G_2-G_1 & G_1-G_0
\end{pmatrix}
$$

Можно заметить, что сумма во всех строках матрицы кроме первой и последней будут равны нулю.

Для правой производной будет следующая формула:

$$\left. \frac{\partial _-^\alpha}{\partial x^\alpha}u(x, t)\right|_ {i} = \frac{1}{\Gamma(2-\alpha)}\frac{d^2}{d x^2} \int\limits_{x}^{b}{\frac{u(s, t)}{(s-x)^{\alpha-1}}ds} = \frac{1}{\Gamma(2-\alpha)} \frac{{}_ -F_{i+\frac{1}{2}}^k - {}_ -F_{i-\frac{1}{2}}^k}{h}$$

$${}_ -F_{i+\frac{1}{2}}^k = -\frac{d}{dx_{i+\frac{1}{2}}} \int\limits^{b}_{x_{i+\frac{1}{2}}}{\frac{u(s, t)}{(s-x_{i+\frac{1}{2}})^{\alpha-1}}ds} = \frac{d}{dx_{i+\frac{1}{2}}} 
\left[ 
    \sum_{j=i+1}^{n-1}{\int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(s-x_{i+\frac{1}{2}})^{\alpha-1}}ds}} + \int\limits_{x_i+\frac{1}{2}}^{x_{i+1}}{\frac{A_is+B_i}{(s-x_{i+\frac{1}{2}})^{\alpha-1}}ds} 
\right]
$$

$${}_ -F_{i-\frac{1}{2}}^k = -\frac{d}{dx_{i-\frac{1}{2}}} \int\limits^{b}_{x_{i-\frac{1}{2}}}{\frac{u(s, t)}{(s-x_{i-\frac{1}{2}})^{\alpha-1}}ds} = \frac{d}{dx_{i-\frac{1}{2}}} 
\left[ 
    \sum_{j=i}^{n-1}{\int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(s-x_{i-\frac{1}{2}})^{\alpha-1}}ds}} + \int\limits_{x_{i-\frac{1}{2}}}^{x_{i}}{\frac{A_{i-1}s+B_{i-1}}{(s-x_{i-\frac{1}{2}})^{\alpha-1}}ds} 
\right]
$$

Имеем по итогу для правой производной:

$$\left. \frac{\partial _-^\alpha}{\partial x^\alpha}u(x, t) \right|_{i} =
\frac{1}{h^{\alpha}} \cdot \left(-G_0 u^k_{i-1} + (G_0 - G_1) u^k_{i} + \sum_{j=i+1}^{n-1}u_{j}^k (G_{j-i} - G_{j-i+1}) + G_{n-i} u_{n}^k \right)
$$

И матрица:

$$ R =
\begin{pmatrix}
G_0-G_1 & G_1-G_2 & G_2-G_3 & G_3-G_4 & G_4-G_5 & \dots & G_{n-1}-G_n & G_n \\
-G_0 & G_0-G_1 & G_1-G_2 & G_2-G_3 & G_3-G_4 & \dots & G_{n-2}-G_{n-1} & G_{n-1} \\
0 & -G_0 & G_0-G_1 & G_1-G_2 & G_2-G_3 & \dots & G_{n-3}-G_{n-2} & G_{n-2} \\
0 & 0 & -G_0 & G_0-G_1 & G_1-G_2 & \dots & G_{n-4}-G_{n-3} & G_{n-3} \\
0 & 0 & 0 & -G_0 & G_0-G_1 &\dots & G_{n-5}-G_{n-4} & G_{n-4}\\
\vdots & \vdots & \vdots & \vdots & \vdots & \ddots & \vdots & \vdots \\
0 & 0 & 0 & 0 & 0 & \dots & G_0-G_1 & G_1 \\
0 & 0 & 0 & 0 & 0 & \dots & -G_0 & G_0-G_1
\end{pmatrix}
$$


---

$$
\begin{pmatrix}
(C_+g_{\alpha, 1} + C_-g_{\alpha, 1} - 1) & (C_+ + C_-g_{\alpha, 2} + V') & C_-g_{\alpha, 3} & \dots & C_-g_{\alpha, n+1} \\
(C_+g_{\alpha, 2} + C_- - V') & (C_+g_{\alpha, 1} + C_-g_{\alpha, 1} - 1) & (C_+ + C_-g_{\alpha, 2} + V') & \dots & C_-g_{\alpha, n} \\
C_+g_{\alpha, 3} & (C_+g_{\alpha, 2} + C_- - V') & (C_+g_{\alpha, 1} + C_-g_{\alpha, 1} - 1) & \dots & C_-g_{\alpha, n-1} \\
\vdots & \vdots & \vdots & \ddots & \vdots \\
C_+g_{\alpha, n} & C_+g_{\alpha, n-1} & C_+g_{\alpha, n-2} & \dots & (C_+ + C_-g_{\alpha, 2} + V') \\
C_+g_{\alpha, n+1} & C_+g_{\alpha, n} & C_+g_{\alpha, n-1} & \dots & (C_+g_{\alpha, 1} + C_-g_{\alpha, 1} - 1)
\end{pmatrix}
$$