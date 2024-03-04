Имеем следующие производные по пространству и производную по времени:

$$\left. \frac{\partial_+^\alpha}{\partial x^\alpha}u(x, t)\right|_ {i} = \frac{1}{\Gamma(2-\alpha)}\frac{d^2}{d x^2} \int\limits_{a}^{x}{\frac{u(s, t)}{(x-s)^{\alpha-1}}ds} = \frac{1}{\Gamma(2-\alpha)} \frac{{}_ +F_{i+\frac{1}{2}}^k - {}_ +F_{i-\frac{1}{2}}^k}{h}$$

$$\left. \frac{\partial _ -^\alpha}{\partial x^\alpha}u(x, t)\right|_ {i} = \frac{1}{\Gamma(2-\alpha)}\frac{d^2}{d x^2} \int\limits_{x}^{b}{\frac{u(s, t)}{(s-x)^{\alpha-1}}ds} = \frac{1}{\Gamma(2-\alpha)} \frac{{}_ -F_{i+\frac{1}{2}}^k - {}_ -F_{i-\frac{1}{2}}^k}{h}$$

$$\left. \frac{\partial_+^\gamma}{\partial t^\gamma}u(x, t)\right|_ {k} = \frac{1}{\Gamma(1-\gamma)}\frac{d}{dt} \int\limits_{0}^{t}{\frac{u(x, s)}{(s-t)^{\gamma}}ds}$$


Потоки определяются следующим образом:

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

$${}_ -F_{i+\frac{1}{2}}^k = \frac{d}{dx_{i+\frac{1}{2}}} \int\limits^{b}_ {x_{i+\frac{1}{2}}}{\frac{u(s, t)}{(s-x_{i+\frac{1}{2}})^{\alpha-1}}ds} = \frac{d}{dx_{i+\frac{1}{2}}} 
\left[ 
    \sum_{j=i+1}^{n-1}{\int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(s-x_{i+\frac{1}{2}})^{\alpha-1}}ds}} + \int\limits_{x_i+\frac{1}{2}}^{x_{i+1}}{\frac{A_is+B_i}{(s-x_{i+\frac{1}{2}})^{\alpha-1}}ds} 
\right]
$$

$${}_ -F_{i-\frac{1}{2}}^k = \frac{d}{dx_{i-\frac{1}{2}}} \int\limits^{b}_ {x_{i-\frac{1}{2}}}{\frac{u(s, t)}{(s-x_{i-\frac{1}{2}})^{\alpha-1}}ds} = \frac{d}{dx_{i-\frac{1}{2}}} 
\left[ 
    \sum_{j=i}^{n-1}{\int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(s-x_{i-\frac{1}{2}})^{\alpha-1}}ds}} + \int\limits_{x_{i-\frac{1}{2}}}^{x_{i}}{\frac{A_{i-1}s+B_{i-1}}{(s-x_{i-\frac{1}{2}})^{\alpha-1}}ds} 
\right]
$$

где $A_j = \frac{u_{j+1}^k - u_j^k}{h}$ - аппроксимация функции линейной по двум точкам.

---

$$
\frac{d}{dx_{p}} \int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(x_{p}-s)^{\alpha-1}}ds} =
\frac{d}{dx_{p}} \int\limits_{h(p-j-1)}^{h(p-j)}{\frac{A_j(x_p - t)+B_j}{t^{\alpha-1}}dt} =
\frac{d}{dx_{p}} \int\limits_{h(p-j-1)}^{h(p-j)}{((A_jx_p+B_j)t^{1-\alpha} - A_jt^{2-\alpha})dt} =
$$

$$=
\frac{d}{dx_{p}} \left. \left[ {(A_jx_p+B_j)\frac{t^{2-\alpha}}{2-\alpha} - A_j\frac{t^{3-\alpha}}{3-\alpha}} \right] \right|_{h(p-j-1)}^{h(p-j)} =
A_j\frac{h^{2-\alpha}}{2-\alpha}\left[ (p-j)^{2-\alpha} - (p-j-1)^{2-\alpha} \right]
$$

$$
\frac{d}{dx_{p}} \int\limits_{x_j}^{x_{j+1}}{\frac{A_js+B_j}{(s-x_{p})^{\alpha-1}}ds} = 
\frac{d}{dx_{p}} \int\limits_{h(j-p)}^{h(j-p+1)}{\frac{A_j(x_p + t)+B_j}{t^{\alpha-1}}dt} =
\frac{d}{dx_{p}} \int\limits_{h(j-p)}^{h(j-p+1)}{((A_jx_p+B_j)t^{1-\alpha} + A_jt^{2-\alpha})dt} =
$$

$$=
\frac{d}{dx_{p}} \left. \left[ {(A_jx_p+B_j)\frac{t^{2-\alpha}}{2-\alpha} + A_j\frac{t^{3-\alpha}}{3-\alpha}} \right] \right|_{h(j-p)}^{h(j-p+1)} =
A_j\frac{h^{2-\alpha}}{2-\alpha}\left[ (j-p+1)^{2-\alpha} - (j-p)^{2-\alpha} \right]
$$

---

Получаем следующие потоки:

$${}_ +F_{i+\frac{1}{2}}^k =
\sum_{j=0}^{i-1}{\left[ A_j \frac{h^{2-\alpha}}{2-\alpha}\left[ (i-j+\frac{1}{2})^{2-\alpha} - (i-j-\frac{1}{2})^{2-\alpha} \right] \right]} + A_i\frac{h^{2-\alpha}}{2-\alpha}\frac{1}{2^{2-\alpha}}
$$


$${}_ +F_{i-\frac{1}{2}}^k =
\sum_{j=0}^{i-2}{\left[ A_j \frac{h^{2-\alpha}}{2-\alpha}\left[ (i-j-\frac{1}{2})^{2-\alpha} - (i-j-\frac{3}{2})^{2-\alpha} \right] \right]} + A_{i-1}\frac{h^{2-\alpha}}{2-\alpha}\frac{1}{2^{2-\alpha}}
$$

$${}_ -F_{i+\frac{1}{2}}^k =
\sum_{j=i+1}^{n-1}{\left[ A_j \frac{h^{2-\alpha}}{2-\alpha}\left[ (j-i+\frac{1}{2})^{2-\alpha} - (j-i-\frac{1}{2})^{2-\alpha} \right] \right]} + A_i\frac{h^{2-\alpha}}{2-\alpha}\frac{1}{2^{2-\alpha}}
$$


$${}_ -F_{i-\frac{1}{2}}^k =
\sum_{j=i}^{n-1}{\left[ A_j \frac{h^{2-\alpha}}{2-\alpha}\left[ (j-i+\frac{3}{2})^{2-\alpha} - (j-i+\frac{1}{2})^{2-\alpha} \right] \right]} + A_{i-1}\frac{h^{2-\alpha}}{2-\alpha}\frac{1}{2^{2-\alpha}}
$$

Выносим общие множители:

$${}_ +F_{i+\frac{1}{2}}^k =
\frac{h^{2-\alpha}}{(2-\alpha)2^{2-\alpha}} \left( \sum_{j=0}^{i-2}{\left[ A_j \left[ (2(i-j)+1)^{2-\alpha} - (2(i-j)-1)^{2-\alpha} \right] \right]} + A_i + (3^{2-\alpha}-1)A_{i-1}\right)
$$


$${}_ +F_{i-\frac{1}{2}}^k =
\frac{h^{2-\alpha}}{(2-\alpha)2^{2-\alpha}} \left( \sum_{j=0}^{i-2}{\left[ A_j \left[ (2(i-j)-1)^{2-\alpha} - (2(i-j)-3)^{2-\alpha} \right] \right]} + A_{i-1} \right)
$$

$${}_ -F_{i+\frac{1}{2}}^k =
\frac{h^{2-\alpha}}{(2-\alpha)2^{2-\alpha}} \left( \sum_{j=i+1}^{n-1}{\left[ A_j \left[ (2(j-i)+1)^{2-\alpha} - (2(j-i)-1)^{2-\alpha} \right] \right]} + A_i\right)
$$


$${}_ -F_{i-\frac{1}{2}}^k =
\frac{h^{2-\alpha}}{(2-\alpha)2^{2-\alpha}} \left( \sum_{j=i+1}^{n-1}{\left[ A_j \left[ (2(j-i)+3)^{2-\alpha} - (2(j-i)+1)^{2-\alpha} \right] \right]} + (3^{2-\alpha}-1)A_i + A_{i-1} \right)
$$

В промежуточном виде имеем:

$$\left. \frac{\partial _ +^\alpha}{\partial x^\alpha}u(x, t) \right|_ {i} = 
\frac{h^{1-\alpha}}{\Gamma(3-\alpha)2^{2-\alpha}} \cdot \left( \sum_{j=0}^{i-2}{\left[ A_j \left[ (2(i-j)+1)^{2-\alpha} -2(2(i-j)-1)^{2-\alpha} + (2(i-j)-3)^{2-\alpha} \right] \right]} + A_{i} + (3^{2-\alpha}-2)A_{i-1} \right)
$$

$$\left. \frac{\partial _ -^\alpha}{\partial x^\alpha}u(x, t) \right|_ {i} = 
-\frac{h^{1-\alpha}}{\Gamma(3-\alpha)2^{2-\alpha}} \cdot \left( \sum_{j=i+1}^{n-1}{\left[ A_j \left[ (2(j-i+1)+1)^{2-\alpha} -2(2(j-i+1)-1)^{2-\alpha} + (2(j-i+1)-3)^{2-\alpha} \right] \right]} + (3^{2-\alpha}-2)A_{i} + A_{i-1} \right)
$$

Введём следующие обозначения:

$$
G_0 = \frac{1}{\Gamma(3-\alpha)2^{2-\alpha}}
$$

$$
G_1 = G_0 \cdot (3^{2-\alpha} - 2)
$$

$$
G_k = G_0 \cdot ((2k+1)^{2-\alpha}-2(2k-1)^{2-\alpha}+(2k-3)^{2-\alpha}); k \geq 2
$$

Получаем:

$$\left. \frac{\partial _ +^\alpha}{\partial x^\alpha}u(x, t) \right|_ {i} = 
\frac{1}{h^{\alpha - 1}} \cdot \left( \sum_{j=0}^{i-2}(A_j G_{i-j})  + G_1A_{i-1} + G_0A_{i} \right)
$$

$$\left. \frac{\partial _ -^\alpha}{\partial x^\alpha}u(x, t) \right|_ {i} = 
-\frac{1}{h^{\alpha - 1}} \cdot \left( G_0A_{i-1} + G_1A_{i} + \sum_{j=i+1}^{n-1}(A_j G_{j-i+1}) \right)
$$

---

$$\left. \frac{\partial _ +^\alpha}{\partial x^\alpha}u(x, t) \right|_ {i} =
\frac{1}{h^{\alpha}} \cdot \left( \sum_{j=0}^{i-2}((u_{j+1}^k-u_j^k) G_{i-j}) + G_0(u_{i+1}^k-u_i^k) + G_1(u_{i}^k-u_{i-1}^k) \right) =
$$

$$
\frac{1}{h^{\alpha}} \cdot \left( \sum_{j=1}^{i-1}u_{j}^k G_{i-j+1} - \sum_{j=0}^{i-2}u_j^k G_{i-j} + G_0(u_{i+1}^k-u_i^k) + G_1(u_{i}^k-u_{i-1}^k) \right) =
\frac{1}{h^{\alpha}} \cdot \left( \sum_{j=1}^{i-2}u_{j}^k (G_{i-j+1} - G_{i-j}) + G_{2} u^k_{i-1} - G_{i} u^k_{0} + G_0(u_{i+1}^k-u_i^k) + G_1(u_{i}^k-u_{i-1}^k) \right)
$$

$$\left. \frac{\partial _ -^\alpha}{\partial x^\alpha}u(x, t) \right|_ {i} =
\frac{(-1)}{h^{\alpha}} \cdot \left( \sum_{j=i+1}^{n-1}((u_{j+1}^k-u_j^k) G_{j-i+1}) + G_1(u_{i+1}^k-u_i^k) + G_0(u_{i}^k-u_{i-1}^k) \right) =
$$

$$
\frac{1}{h^{\alpha}} \cdot \left( \sum_{j=i+1}^{n-1}u_j^k G_{j-i+1} - \sum_{j=i+2}^{n}u_{j}^k G_{j-i} + G_1(u_i^k-u_{i+1}^k) + G_0(u_{i-1}^k-u_{i}^k) \right) =
\frac{1}{h^{\alpha}} \cdot \left( \sum_{j=i}^{n-1}u_{j}^k (G_{j-i+1} - G_{j-i}) + G_{0} u^k_{i-1} - G_{n-i} u^k_{n} \right)
$$

---

Итого имеем:

$$\left. \frac{\partial _ +^\alpha}{\partial x^\alpha}u(x, t) \right|_ {i} =
\frac{1}{h^{\alpha}} \cdot \left(-G_{i} u^k_{0} + \sum_{j=1}^{i}u_{j}^k (G_{i-j+1} - G_{i-j}) + G_0 u_{i+1}^k \right)
$$

$$\left. \frac{\partial _ -^\alpha}{\partial x^\alpha}u(x, t) \right|_ {i} =
\frac{1}{h^{\alpha}} \cdot \left( G_{0} u^k_{i-1} + \sum_{j=i}^{n-1}u_{j}^k (G_{j-i+1} - G_{j-i}) - G_{n-i} u^k_{n} \right)
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

И матрица для правой производной будет иметь следующий вид:

$$ R =
\begin{pmatrix}
G_1-G_0 & G_2-G_1 & G_3-G_2 & G_4-G_3 & G_5-G_4 & \dots & G_{n}-G_{n-1} & -G_n \\
G_0 & G_1-G_0 & G_2-G_1 & G_3-G_2 & G_4-G_3 & \dots & G_{n-1}-G_{n-2} & -G_{n-1} \\
0 & G_0 & G_1-G_0 & G_2-G_1 & G_3-G_2 & \dots & G_{n-2}-G_{n-3} & -G_{n-2} \\
0 & 0 & G_0 & G_1-G_0 & G_2-G_1 & \dots & G_{n-3}-G_{n-4} & -G_{n-3} \\
0 & 0 & 0 & G_0 & G_1-G_0 &\dots & G_{n-4}-G_{n-5} & -G_{n-4}\\
\vdots & \vdots & \vdots & \vdots & \vdots & \ddots & \vdots & \vdots \\
0 & 0 & 0 & 0 & 0 & \dots & G_1-G_0 & -G_1 \\
0 & 0 & 0 & 0 & 0 & \dots & G_0 & G_1-G_0
\end{pmatrix}
$$