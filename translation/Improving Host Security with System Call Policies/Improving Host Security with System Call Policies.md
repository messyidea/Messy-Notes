# 通过系统调用策略来提高宿主机的安全性
## 摘要
　　我们在这篇文章中介绍了一个可以测量程序运行所需要的权限的系统。通过我们的系统，程序在运行过程中如果需要提高执行权限，会被一些可配置的策略给终止，去除了设置suid和sgid权限位的需要。我们提出的“systrace”的设计和架构支持细粒度的过程约束，入侵检测，审计和测量。它还促进了复杂进程的策略生成。通过“systrace”，我们有可能在训练中自动对程序生成策略或者在程序运行的过程中交互性地生成规则。这些规则描述了我们对一个程序或者服务在系统中运行的权限的要求，强制控制程序不去执行不被允许的操作。我们表明，“systrace”高效，不会对程序的性能造成影响。

### 1. 介绍
　　如今互联网上有越来越多的商业举动，所以计算机安全也变得越来越重要。尽管通过了数十年的研究，我们始终无法制造安全的操作系统，或是测量它们的安全性。

　　我们认为，应用程序总是会包含一些可利用的漏洞，导致一些未经授权的访问。对于一个产品，竞争对手可能会在远程或者本地滥用这些漏洞。为了提高操作系统的安全性，我们尝试在应用程序上层部署一些不同的安全层，希望其中的一个层能够抵御住恶意的攻击。这些层包括用防火墙来严格控制网络权限、操作系统原始的非可执行堆栈以及应用层的特权分离保护。在理论和实践中，我们发现，通过增加安全层来规避应用的攻击是可行的。

 　　防火墙能够防止远程登录和加强权限控制。比如对于一个网站服务器，如果一个敌人成功的找到了一个服务器的漏洞，那么在之后的攻击中，他可能利用这个漏洞来获取更多的权限。因为能够本地访问操作系统，敌人可能获取到root权限。这样之后敌人可能获取到访问本地局域网的权限或者可以进行特殊的系统调用。

　　如果要从这个安全性的破坏中快速地恢复过来，最重要的是检测入侵和保持对入侵的踪迹分析。尽管现在有很多入侵检测系统能够通过流量分析或者系统调用分析来推断入侵，小心的入侵者也有可能回避它们。

　　与其探测入侵，我们可以尝试限制敌人来限制敌人所能够造成的危害。对于文件系统，访问权限管理能够让我们决定哪些文件能够被读和哪些文件能够被写。尽管ACLs比传统的unix权限模型更加通用，但是他们不能够完全地限制敌人，而且配置起来比较繁琐。

　　我们发现要持久性地改变系统唯一的途径是通过系统调用。系统调用是通往内核特权操作的大门。通过监视和严格控制系统调用，我们就可以保证一个应用程序不会对系统造成危害。基于系统调用介入的解决方案已经在过去被提出了。 系统调用介入这个的方案能够允许系统探测哪些操作是违反要求的，并且在它们执行的时候阻止它们。然而，如何制定精确的策略仍然是一个问题。

　　本篇论文提出了Systrace——一个能够有效地限制多种应用程序，支持多策略，支持交互式策略生成、操作探测和保护、有良好的log的解决方案。我们还提出了一个叫做权限提升的的新奇的途径去减少应用程序的权限需求。我们探讨Systrace的设计和实现，来说明它是一个可扩展的、高效的主机安全保护方案。

　　本文的剩下部分是这么安排的：第二节讲述了相关的工作，第三节我们提供了我们工作的更长远的动机，第四节我们讲述了Systrace的设计，第五节我们讲述了Systrace的实现，第六节我们对这个系统进行了测试，第七节我们细节地展示了系统的性能，第八节我们讲述了今后的工作，第九节是总结。

### 2.相关工作
　　尽管功能和权限控制列表是继承传统的unix权限控制模型，并且提供了细粒度的控制，他们不能够阻止不被信任的应用程序造成的危害，相反的，我们使用基于系统调用截获和干涉的结构可以阻止成功侵入所带来的危害。

　　Goldberg实现的Janus是第一个系统调用干涉工具。它使用了ptrace和/proc 结构。Wagner说ptrace不是一个合适的系统调用干涉的接口，竞态条件的接口允许敌人完全从沙箱中逃脱。原始的Janus实现有几个缺点：程序不能够改变它们的工作目录或者执行chroot命令，因为chroot不能够捕获到用户程序切换状态。随着时间的推移，Janus发生了重大的变化，最近的版本用了类似Systrace的混合的方法去直接控制操作系统的系统调用。

　　限制应用的一个特别困难的问题是符号链接。他可以重定向到文件系统的任意路径。Garfinkel说安全的调用序列不应该跟着符号链接。他的方法使用了一个针对linux操作系统系统调用的扩展，但是这个扩展阻碍了任何访问带有符号链接的文件名的系统调用。Systrace通过文件名正规化和参数替换解决了这个问题。现在，Janus还不支持入侵检测、审核和规则生成。

　　Jain和Sekar提供了另一个相当完善的系统调用介入的处理方案。在一些系统里面他们的方案基于ptrace和suffers实现，但是遇到了上述的问题。而且，他们不解决命名模棱两可的问题所导致的策略逃避。因为他们使用c++作为它们的策略语言，创建全面的策略是十分困难的。Systrace，在另一方面，提供了自动化的互动式策略生成，能够允许我们在复杂的环境中快速创建策略。

　　还有一些系统也使用了类似系统调用介入的结构，比如BlueBox, Cerb, Consh, MAPbox 和 Subterfugue。

　　Peterson等人提出了一个通用的系统调用api来限制不信任的程序。这个API非常灵活，但是没有提供记录审计和入侵检测。此外，制定安全策略的工作量和写一个沙箱一样大，需要进行编程。

　　Domain Type Enforcement是一个内核级别的去重新限制每个进程的访问权限的方法，它依赖于每个进程的范围。一个完全的DTE实现需要对操作系统进行多方面的修改，而且不能够自动地去适应新的子系统。因为策略在系统启动的时候就被锁住了，所以用户不能够创建自己的策略。对比于Systrace，DTE的范围是不区分user的。我们感觉系统调用干涉可以更加灵活。它能够允许我们设计和创建简单的系统并且支持策略生成，记录审计，入侵检测等等。

　　Flask微内核的安全架构强调了策略的灵活性。它反对系统调用干涉结构，称本质上限制了策略的灵活性。相反，Flask系统给每一个元素赋予了安全标记，然后部署了一个安全服务来判断是否安全。然而，Systrace通过了混合设计能够允许我们战胜传统系统调用介入方法的不足，关于这个可以看第六节。

　　SubOS采用了一个基于元素标记的类似的方法去限制系统权限。根据对象的来源，它们被分配不同的子用户标记。一个进程访问一个对象的权限继承了这个子用户标记。这样的话，一个进程因为一个恶意对象而被结束所造成的危害是有限的。在测试中，只有少数的应用程序能够因为这个而会被毁灭，对这些应用程序执行安全策略足以防止恶意数据造成破坏。

　　Forrest 等人分析系统调用序列来区分不同的进程。他们的工作延续了Hofmeyer等人。通过记录应用程序的系统调用并和好的系统调用数据库比较实现了一个入侵检测系统。异常序列表示一个正在进行的入侵。通过训练进程获取一个好的系统调用序列和Systrace自动生成规则的特性是类似的。Wespi等人通过边长模式审计事件扩展了这个方法。尽管分析系统调用日志或者审核序列是检测入侵的有效的结构，但是不能够帮助我们去组织这些有危害的系统调用。最近的研究也展示了拟态攻击(mimicry attacks) 能躲避基于系统调用序列的系统调用检测。Systrace步进能够检测这些入侵，还能够阻止它们或者至少限制它们所能够造成的危害。另外，在第六节里面也说明逃避攻击(evasion attacks)是不可能的。
　　
### 3.设计动机
　　大多数运行在计算机操作系统中的应用程序都太过复杂，太难以去信任：网络浏览器，服务器等。甚至当我们有查看源代码的权限，也很难去分辨是否这些应用程序是安全的。它们可能包含恶意代码或者是可利用的漏洞。

　　因为我们不可能找出所有的弱点，我们假设程序存在被敌人知道的若干问题，敌人可以利用这个问题去获取到未经授权的权限。

　　我们通过限制程序的操作来限制敌人通过这个漏洞所造成的影响。通过观察系统调用，捕获到了异常，操作系统自然的去选择在系统调用层面强制限制应用程序。

　　一个应用程序被一组安全策略限制。定义一个正确的策略是困难的，我们不可能得到所有应用程序的源码。因此我们需要更加直观且更加容易表达的语言来表达策略。我们必须能够实现在不太了解这个应用程序的情况下生成对应的限制策略。

　　我们可以采用安全策略作为规范用来描述应用程序期望的行为。当检测到应用程序一个操作即将被执行，任何一个背离规则的的行为都被视为一次安全妥协。为了促进入侵分析，我们也希望能够记录应用程序之前的相关操作。

　　经验说明，敌人通常通过滥用setuid和setgid来提升他们的权限。这些程序执行着比用户启动它们时的权限更高级的权限。尽管提高权限也是为了正确的操作所必须的行为，setuid这个模型的粒度太粗了。我们的目标是提供一个细粒度的模型去消除对setuid的需求，并且集成一个方法去提升权限。

　　Systrace实现了这些目标，有效地提升了宿主机的安全性，减少了敌人利用漏洞所带来的危害。下一节将讨论Systrace的设计。

### 4.设计
　　下面有几种途径去实现系统调用干涉。我们会使用已经存在的拦截结构去实现，这个拦截结构可能是一个完全用户空间的，或者是完全内核空间的，或者是用户空间和内核空间混合的。用户空间的实现是可移植的，但是可能会对性能造成很大的影响，此外，拦截结构可能不能够提供要求的安全保障，在用户空间难以跟踪操作系统状态，比如说进程的存在或者是forking。一个显著的例外是SLIC,一个通过系统调用介入创建的操作系统扩展。不幸的是，它是不可移植的，而且给操作系统添加了显著的复杂性。

　　相反的，一个完全在内核层的实现可能会很快，但是会变得不易移植，而且也会给操作系统带来显著的复杂性的提升。

　　我们采用了一种混合的方法，在内核层实现了系统的一小部分。内核曾能够支持对系统调用的快速允许或者阻止。这样做基本没有性能损失，因为不需要切换到用户空间去询问用户空间的一个服务这个操作是否需要禁止。

　　在内核的一些控制也允许我们进行故障防护。即使有一些不可预见的问题可能导致监视器自己结束，但是也依然没有应用程序能够逃脱它的沙箱。当沙箱进程结束，内核会结束所有它所监视的进程。总之，内核监视着新进程的产生和结束。子进程也会继承父进程的限制策略。

　　如果内核部分不能够使用，那么它必须询问在用户空间的限制服务，让它去做抉择。在这种情况下，进程会被阻塞，直到服务返回一个决定。信息会通过简单而又全面的接口从内核空间输出到用户空间。

　　用户空间策略服务通过内核接口来监控进程。或者通过它获取等待决策的信息和进程状态变化。进程状态变化包括进程是否已存在，进程改变其uid或gid等等。

　　这个服务也会请求系统调用是否成功的信息。这些东西允许我们的到很多信息,比如进程切换是否成功，这个信息会在策略数据库中增加一条规则。

　　系统调用拦截不会提供在策略决定和系统调用执行之前的原子性。这样一来，敌人可以在它执行之前改变系统调用，在这之后策略服务才会检测它。举一个例子，两个进程共享它们的地址空间能够合作提交一组系统调用参数给策略服务和进程。当内核使第一个进程休眠去询问策略服务，第二个进程仍然继续运行可能在第一个进程被检查之后改变第一个进程的系统调用参数。对于文件系统访问权限，敌人可以把文件名改为符号链接重定向到任意位置。缺少原子性可能会导致敌人从沙箱中逃脱。

　　 我们通过替换系统调用参数，把这些参数替换为经过Systrace评估过的参数来防止上述情况的放生。被替换的参数可以通过在内核空间的一个预留的只读的buffer让监控程序探测到。这确保了内核执行的所有参数都是经过策略限制的。

　　在限制程序做决定之前，系统调用以及它的参数会转化为可读性的格式，这些操作都会自动完成，不需要让用户完全知道系统调用具体的语义。

#### 4.1 策略
　　现存的框架已经有一些通用的策略语言以及策略的评价方法，但是在我们看来都太复杂了。所以，我们创造了新的策略语言和评价方法。我们创造的方法也被其它沙箱工具采用了。

　　我们使用了一个有序的策略列表，每个系统调用都是一个策略。一条策略申明是一个布尔表达式，并和一个动作语句结合起来。合法的动作有询问，阻止，允许等。如果布尔表达式为真，那么这个动作就会被执行。询问会请求用户到底是阻止还是允许该操作。

　　一个布尔表达式包括变量和逻辑符号（and，or and not），变量是一个元组(subject op data)，subject是系统调用的名字，data是系统调用参数，op是一个函数，这个函数以subject和data为参数，返回true或者false。

　　所有策略的集合就是安全策略。对于一个系统调用，策略从第一个系统调用链表开始度量，从第一个布尔表达式返回true之后结束。（见表格1）这个行为决定了这个系统调用到底需要允许还是阻止。

　　如果没有布尔表达式为真，那么这个决策会自动提交给用户或者根据设置自动阻止。4.2节会详细解释我们怎么让策略自动或者交互式地生成。当一个系统调用被阻止，我们也能从监控程序得到错误码。

　　为了给不同用户创建完备的规则，决策语句可能会包含很多谓词。一个策略如果被采用，要所有的谓词都匹配，否则就无视这条策略。通过谓词，可以让一个用户的策略更加严格或者宽容。谓词被添加到策略语句中，形式是if{user,group} op data, op是等于或者不等于，data是用户名或者组名。

```
    Policy: /usr/sbin/named, Emulation: native native- sysctl: permit
      native-accept: permit
      native-bind: sockaddr match "inet-*:53" then permit native-break: permit
      native-chdir: filename eq "/" then permit native-chdir: filename eq "/namedb" then permit
      native-chroot: filename eq "/var/named" then permit native-close: permit
      native-connect: sockaddr eq "/dev/log" then permit

      ---Fig1
```

　　日志修改器会被加入到策略语言去记录匹配的系统调用。每次当一条系统调用被策略匹配了，系统会记录该系统调用所有的信息和决策的结果。这允许我们去任意细粒度地审核。

#### 4.2 策略生成

　　通常创建规则的是希望把应用程序放到沙箱中运行的用户。策略生成不是一件简单的任务，因为一些策略语言非常复杂。尽管这些语言表达性很好，但是创建一个好的规则也相当有难度。

　　我的把一个好的规则定义为能使应用程序执行他们所必须要执行的操作，而阻止一切其它的操作。

　　很明显，我们能够创建出一个规则满足应用程序所有可能做的正常的行为，如果一个行为没有在这个规则内部，那么这个行为就不被允许。

　　接下来，我们将展示为什么我们的策略语言方便了策略的生成。策略语言设计得很简介，每个策略语言都能够自解析，因此很容易被新的策略语句给扩展。这种方法的最大的有点就是可以让策略进行迭代。

　　我们通过运行程序，记录程序运行中所使用的系统调用来自动生成规则。我们把系统调用参数正则转换到策略语句中。当应用程序尝试在测试的时候执行系统调用，它会自动与前面的规则比对，看是否重复。如果不重复，则把规则添加到策略语句中。不同于入侵检测系统只分许系统调用名称序列，我们的策略语句会捕获系统调用的完整语义，确保了安全。

　　应用程序在随后的运行中，前面的规则会被自动地采用。对于那些创建任意名字的应用，我们有必要自己修改配置文件，去处理这些不确定性。

　　当自动生成规则时，我们假设应用程序本身不包含恶意代码，且只和良性数据操作。否则，规则可能会允许不被期望的操作。

　　如果这个假设不成立，我们使用交互性规则生成。交互需要用户的决策。当一个决策需要由用户判断，用户只需要根据图形界面，按下特定的按钮即可。用户可以增加一条记录，也可以选择拒绝这个系统调用。

![图形界面](http://i4.tietuku.com/e32ae00553385007.png)

　　如果我们不能够跑所有的代码，自动生成规则不能够包含所有合法的行为，那个这个规则则不是一个优秀的规则。然而，它提供了一个最基础的规则覆盖了一些必要的安全的操作。在交互生成策略的同时，我们会反复细化策略，直到策略完善。

　　系统在开始的时候给用户提供了一些基础的策略模板供用户使用。每当一个初始的规则被指定，规则当且仅当在预配置文件找不到相应规则时才会添加。每次要求增加规则，表明一个位置的系统调用正在发生，用户可以允许、阻止或者调查它。

　　当一个安全的策略已经敲定，策略会被自动部署。在这种情况下，当一个新的系统调用执行时，不会询问用户，而是直接阻止它，并且返回错误信息。这个不成功的行为会被记录到日志文件中。

#### 4.3 特权提升
　　有一些软件会存在超出自己特权的行为，在某些情况下我们需要增加它的权限。在Unix中，有很多系统服务或者软件需要请求root权限才能运行，通畅，只有少部分系统调用需要用的特权。我们分配特权的原则是尽量让软件通过最少的特权来完成所有工作。

　　为了确定哪些行为需要特权，我们扩展了策略语言，给每条策略语句都非配了一定的权限。Systrace会在程序启动的时候给予程序最小的权限，只有当系统调用需要特权的时候它才会得到特权，当那个系统调用结束时就释放特权。

　　因为可能每个用户都会启动他们自己的策略服务，权限提升只有在Systrace策略服务工作在root的情况下才会发生。否则，有可能敌人可以以另一个用户的名义启动他自己的策略。识别setuid或者setgid的操作允许我们对这些操作进行权限提升而不需要使整个应用程序都工作在特权模式。这样导致的结果是，即时敌人控制了一个危险的应用，他也只能够拥有极小的权限，根本就不能造成破坏。

　　ping这个程序，是一个有setuid操作的程序，它需要特权去操作。为了发送和接收ICMP包，ping会创建一个原始套接字，这个操作需要特权。在这种情况下，我们只需要设置策略在ping创建原始套接字的时候给予其特权就行了。

　　Unix允许一个应用通过改变uid和gid放弃特权。这个改变时持久的，也就是说在这之后进程讲不能够恢复其特权。如果一个程序偶尔需要特权，那么丢弃特权不是一个好的方法。在这种情况下，特权提升将变得特别有效。比如，ntpd服务会同步系统时钟，改变系统时间是一个需要特权的操作。最近的远程root权限漏洞可以通过单一系统调用权限提升来得到预防。


### 5.实现
　　我们现在来讲解一下Systrace的实现概要。Systrace现在已经能够在Linux，Mac OS X, NetBSD 以及 OpenBSD上运行。我们专注在OpenBSD上的实现。

　　为了帮助说明我们实现的安全性，简单是我们首要的考虑因素。我们把不同的功能集成到各自的抽象组件来使实现变得简单。我们可以从图3看到系统大致的架构。

![图3.架构](http://i4.tietuku.com/7d2cdec8f075a40a.png)

　　当监控程序发现应用执行一个系统调用，内核查询在内核中的一个小的策略数据库去判断这个操作应该允许还是阻止。在这种情况下，策略不会去检查任何系统调用参数。通常，读和写的操作是被允许的。内核通过与/dev/systrace 设备交流去请求策略服务做判断。

　　当进程有不同的策略，所有的系统调用策略将被推送到策略服务来进行。当内核等待结果的时候，进程会休眠，直到得到结果。如果途中有其它进程把该进程唤醒，那么内核会阻止该系统调用，然后返回一个错误信息。为了确保同步，每个内核的信息都会带有一个序列号，所以用户空间策略判断信息会和请求一一对应。序列号确保了用户空间决策服务不会反馈到错误的请求中去。

　　当用户空间的策略服务接收到一个需要判断的请求，它会查看与进程相关的策略，然后翻译系统调用参数。为了翻译这些参数，我们为每个系统调用参数注册了一个转化器。比如
```
socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)
```
会转化为下面这种形式
```
socket: sockdom: AF_INET, socktype: SOCK_RAW
```
第三个参数没有被转化，因为它和Unix system不相关。

　　很多参数的转化器非常简单，但是转化文件名却异常复杂。在unix中文件名和程序的工作目录相关。为了转化文件名到绝对路径，我们需要知道当前进程的工作目录，还需要知道它的chroot环境。总之，所有的符号链接都需要被转化，这样敌人才不能够通过符号链接来获取路径访问权限。

　　转化器也起到了标准化参数的作用。参数替换框架被用于替换原始的参数，内核只会看到已经被标准化的参数，所以敌人不能够通过使用误导性的参数来躲过策略。在恢复系统调用之前把标准化的参数写入内核中的缓冲区使得替换参数成为可能。此外，我们不允许在所有过程中包含符号链接，因为标准化后的文件名不会包含任何符号链接。

　　一条允许创建原始套接字的策略可能会长这样：
```
socket:  socktype eq "SOCK_RAW" then permit
```

　　在布尔表达式里面的操作会被转化为人类刻度的字符串。我们现在支持eq，match，re和sub作为操作符。

+ eq操作符为真当且仅当系统调用参数和策略表达式完全匹配。
+ match操作表示文件被找到。它能被用在系统调用文件名替换中。
+ re操作通过正则表达式去匹配系统调用参数。它很通用，但是会比其它操作更费时间。
+ sub操作在系统调用参数包含在集合中的时候返回true

　　如果当前系统调用被抉择为拒绝或者允许，策略服务会把结果返回给内核，然后唤醒睡眠的进程。否则，监视器会询问用户怎么去抉择。通知机制可以被独立地实现，不管用图形界面还是文字界面都是可以的。用户也能够通过图形界面添加新的规则。

　　系统访问文件系统的策略是类似的。比如access，stat和lstat的功能是相同的。为了避免规则的重复，我们引入了一个aliasing去把相同的系统调用映射为同一个系统调用，这个系统调用被用在策略语言中。目前，fsread被用于读文件系统提升权限的系统调用对象。fswrite被用于写文件提升权限的系统调用对象。open系统调用会根据其参数映射为fsread或者是fswrite。alising减少了策略的体积，简化了策略的生成。

　　我们有可能通过谓词来让策略变得更加灵活。策略声明当且仅当谓词匹配时才有效。比如，为了防止通过ssh获取到root权限，可以用一条策略声明shell只能够被非root用户使用。为了跟踪进程的uid和gid，当进程uid和gid放生改变时，内核会给策略服务发送信息。

　　execue这条系统调用会被特殊对待。当一个进程执行林外一个程序，它的内存镜像会被新启动的进程给替代，为了能够支持细粒度的策略，我们可以给进程设置新的策略。这个策略可以根据被执行的程序来获得。所以，一个Systrace可以同时给多个进程设置多个规则。

　　不同的程序策略会像分来的文件一样存放。用户可以在用户自己设置的目录中保存规则。管理员也可以给所有用户提供全局的规则。为了让程序运行在沙箱中，用户要通过命令行用沙箱的命令来启动应用程序。管理员可以全局添加策略保证用户的程序都收到这些策略的约束。

### 6. 分析
　　如果敌人拿到了沙箱的权限，它可能会尝试通过写入策略欺骗策略服务，让危险的操作返回允许的结果。尽管现在沙箱都有共同的问题，对于这些问题，我们提供了新的解决方法，然后并讨论系统调用拦截固有的局限性。

#### 6.1 安全分析
　　为了让安全规则成功应用在系统调用上，我们需要面对如下挑战：不正确的复制语义，资源别名，缺少原子性，影响系统调用。我们简单地说一下他们的本质，然后讲一下我们怎么解决它们。

　　沙箱工具如果要做到决策，必须能够跟踪操作系统的状态。Systrace需要在进程执行时跟踪进程的uid和名字。为了避免不正确地复制系统的语义，我们的内核层的实现会把所有相关的状态变化传送给策略服务进程。

　　资源别名会导致多个地址能够访问同一个资源。在Unix系统中，我们有可能通过与系统服务交流获取权限或者是使用软链接来让不同路径访问到同一个资源。敌人可能通过这些不直接的途径来躲过沙箱的策略从而获取非法权限。系统调用干涉机制不能够察觉到系统服务允许代理来操纵系统资源。当允许沙箱程序和系统服务交流的时候，我们需要考虑后果。然而，我们可以通过符号链接和相对路径来预防别名。

　　另外一个问题是在检查的时间和实用的时间中缺少原子性。这可能会导致资源名字映射的资源在决策和系统执行的时间之间被修改。敌人可能通过这个问题来访问到不同的资源。比如：共用一块内存的合作进程可能会导致系统调用参数的重写。

　　Systrace通过标准化系统调用参数解决了别名问题和原子性问题。我们给操作系统提供了标准化的值，这导致了资源名字映射的值不能够被敌人修改。对于文件名，这解决了软链接和相对路径的问题。唯一的例外是readlink的系统调用，这是我们不能解决的最后一个元素。

　　为了解决文件名含有符号链接的问题，内核拒绝监视有任何符号链接的进程。我们不像MAPbox一样把重写的参数放在栈中。我们在内核中提供了一个只读的空间。如果不是这样，多线程应用就能够在决策完成之后修改系统调用参数的值。

　　这样做了以后，将不可能发动躲避攻击。系统调用只会在匹配策略之后才能执行，否则就会被拒绝。

　　我们也需要把拒绝系统调用的副作用考虑在内。我们假设安全策略是完全正确的，当且仅当应用程序超出它的权限范围的时候系统调用才会被阻止。由于许多应用程序的行为取决于返回给他们的错误代码，我们可以把错误代码制定为Systrace策略的一部分。每个系统调用都拥有自己的一组有效的返回代码，而不是只有EINTR或者EPERM。为了避免让应用程序不知所措，我们允许策略可以设置它们自己的错误代码，而不只是规定的值。比如说，我们在认为应用程序不允许访问某个文件的时候让内核返回EACCESS，另一方面，我们可以返回ENOENT让应用程序认为该文件不存在。

　　此外，我们实现了安全过程分离和策略开关，这些问题经常被忽视。当一个已用程序执行了一个新的程序，操作系统将替换该进程正在执行的代码，并执行新的程序。如果新程序可信任，我们可能希望监视器不要再去监视这个程序。另一方面，新的程序也意味着新的功能，如果能够替换规则那就再好不过了。如果有请求，Systrace会报告返回的值，这样能让我们知道这个新程序是否执行成功。在返回的结果中，success表示监视进程正在跑一个新的程序，我们允许策略制定是否需要关闭规则还是让新的规则生效。当这些改变生效后，进程继续执行。

　　系统的安全依赖于文件系统的完整性，我们假设它是安全的。如果敌人能够控制文件系统，他可能会修改策略，允许一些恶意的系统调用或者给一些可信任的程序加上阻止策略来妨碍它们的运行。

　　审计跟踪可以生成策略修改的log。比如审计跟踪可以记录一个用户执行的所有命令。这足以让Systrace记录他的shell和所有执行的命令。

　　特权提升的好处是可以减少一个应用程序执行需要的权限。应用程序本来需要root才能够执行现在只需要在特定的系统调用中提升特权秀可以了。另外一些系统调用是以用户赋予它的权限来执行额。setuid的语义可以阻止用户通过ptrace调试特权应用程序，我们在应用程序特权提升时也使用相同的语义。

#### 6.2 策略生成
　　策略生成经常是一件被忽视的事情。为了让沙箱工作正常，我们需要一个规则既能够限制软件的权限，又能够不去阻碍它的功能。为了促进策略的生成，我们的策略语言允许策略通过增加新的策略语言来完善自己。

　　我们可以通过记录软件正常的行为来自动生成策略。每次我们捕获到软件的行为，我们会判断这个行为有没有在策略中被记录，如果没有的话，就新增一条策略上去。

　　通过这样产生的策略涵盖了应用程序所有执行的代码。如果应用程序有随机的参数，我们会给这些参数做独立处理。

　　比如，当mkstemp("/tmp/confXXXXXX")  这条命令创建文件 “/tmp/confJ31A69”，自动策略生成会增加这么一条策略：
```
fswrite: filename eq "/tmp/confJ31A69" then permit
```

　　post进程会改变策略语句把这些随机的参数变得独立，从而适用于之后的语句。
```
fswrite: filename match "/tmp/conf*" then permit
```

　　自动策略生成会遇到和Hofmeyr等人在软件正常行为分析中相似的问题。我们需要确定在训练的过程中软件没有异常的行为，并且需要尽可能覆盖软件所有的代码。然而，自动生成策略和交互式生成策略是结合在一起的。我们不需要在训练中覆盖软件完整的行为，当软件的系统调用没有被策略覆盖到时，策略服务会自动询问用户，并且把新的决策加入到策略中。

　　monkey.org 证明了我们的方法是可行的。这个网站是一个shell提供商，他使用了Systrace去服务了200度用户。他们为250多个应用程序生成了独立的规则。

　　一个调查表明，一个应用程序平均会使用71个不同的系统调用。见表格4。通常Unix系统有几百个系统调用。当一个敌人控制了一个应用程序，她可能想通过可能的系统调用获取更高的权限。通过限制应用程序的系统调用，我们可以减少她所造成的伤害。

![图4](http://i4.tietuku.com/bb032a92fa71c38c.png)
　　从图中我们可以看到两个峰。一个在64还有一个在87.第一个峰是因为一些类似的标准unix系统调用造成的，比如chmod，cat，rmdir和diff。第二个峰是由于不同的系统工具造成的，这些工具会需要更多的系统调用，比如网络连接或者是创建文件。

#### 6.3 入侵检测和保护

　　入侵检测和预防的能力是和架构相关的。被阻止的系统调用会被操作系统自动记录到日志中。这可以防止敌人对系统造成危害，并且给这个行为创建了一个警告。

　　一个正确的策略会限制应用程序只能做他们本来做的事。虽然这可以防止一个对手任意破坏操作系统，但是她也可能滥用应用程序固有的功能，导致系统的破坏。我们采用审计跟踪记录潜在的恶意活动，而不是通过策略阻止。

　　在monkey.org, 142个用户在两个月的时间里产生了约350000条日志。该系统被配置为记录所有的阻止日志以及exec和connect。通过将所有用户的策略违反数量和执行程序的数目相关联，我们可以确定哪些用户表现了不寻常的行为。在图5中，我们注意到一些用户产生了远超其它用户的违反日志。在这种情况下，策略限制最多的是用cgi脚本创建用户。用户执行最多并且被策略允许的行为是查看邮件。

![图5](http://i4.tietuku.com/bb032a92fa71c38c.png)

#### 6.4 局限性
　　虽然系统调用干涉这个方案非常强大，但是也存在着其固有的局限性。检测系统调用序列不能完全知道应用程序的内部状态。比如，系统服务可能会在进程成功认证之后改变进程的权限，但是如果认证失败会拒绝其它的权限。在系统调用层的杀项工具不能够记录这些状态的变化。然而，仍然有可能约束状态变化，比如，root不应该被允许登录。这是可能的，因为这些约束不依赖于应用程序的内部状态变化。

　　为了提高带有认证服务的安全性，可以把权限分离和系统调用策略结合起来。通过权限分离，应用程序主体会执行在非特权的进程上下文，在没有权限的路径入侵不会导致权限的扩大。然而，在unix系统中，非特权的进程仍然可以执行系统调用允许访问本地网络。通过Systrace，我们可以阻止非特权进程执行不必要的系统调用。

### 7 性能

　　为了确定Systrace对性能的影响，我们测试了几个应用程序的单系统调用时间消耗，冰爷所有的测试都至少重复5次。所有的测试都在一台1.14GH的奔腾3处理器中进行的。显示的结果是响应的平均时间和标准偏差。

　　我们制造了一个microbenchmarks，这个东西可以把同一个系统调用重复十几万次，并测量real time,system time和user time。系统调用的平均时间大约是每次重复的平均时间。

　　作为基准，我们衡量在没有监视情况下的geteuid的系统调用时间。我们把这个时间与运行在Systrace的两种不同策略的geteuid的系统调用时间相比较。第一种策略是在内核层允许系统调用，第二种策略是内核询问策略服务来判决。我们可以发现，内核允许系统调用大约会给每个系统调用增加31%+-3%的时间，这些增加的时间是花在内核中的。当内核需要到用户空间去询问策略服务是，执行单个系统调用使用的时间会更长，这大多是因为两次从内核态到用户太的切换。结果显示在表6

![图6](http://i4.tietuku.com/4f197a05cb04f788.png)

　　开放的系统调用需要在内核层做更多的工作。microbenchmark显示内核决策会增加大约7%的执行时间。如果需要策略服务决策，则执行的时间取决于文件路径的深度。当一个文件名作为系统调用参数，增加的时间大约是25层文件路径深度的时间。每个文件名参数会增加大约30微秒，这些时间主要花在文件名的正规化中。结果显示在图7。

![图7](http://i4.tietuku.com/bb194a5baf58d061.png)

　　microbenchmark 最后测量的是随意在/dev/arandom读取1kb所使用的时间，内核层的判断基本上不会对系统调用的执行时间产生影响。我们忽略了对用户空间的测量，因为读操作不需要去询问用户空间。结果显示在图8。

![图8](http://i4.tietuku.com/47c142075450c03d.png)

　　强制系统调用策略会增加应用程序执行的时间，但是平均影响很小。

　　图9显示了gzip压缩500kb到5mb时间的比较。基本上gzip 每秒执行3次系统调用，他们大多数是读和写。在这种情况下，Systrace对于性能的影响基本上是可以忽略不计的，因为程序把时间主要花在计算上，而不是系统调用上。

![图9](http://i4.tietuku.com/8d565b6a359bae5e.png)

　　为了评估一个应用程序频繁读写文件对性能造成的影响，我们写了一个和Andrew benchmark类似的benchmark 。它由下面几个部分组成：拷贝tar文件到Systrace目录，解压，运行configure，编译，并删除所有源码。

　　在benchmark的测试中，一共有44个程序被执行。我们使用Systrace自动生成策略，然后用一个脚本增强策略。这次测试大约执行了127000个系统调用，分解出来的最频繁的系统调用可以见图10.最频繁的系统调用是因为分配内存而导致的中断，访问文件的系统调用也很突出。

![图10](http://i4.tietuku.com/ae64540a9143e660.png)

　　我们可以在图11看到一个直观的比较。在Systrace下，我们发现增加了大约31%的执行时间，执行的系统调用增加到了大约726000个，因为规范化文件名需要执行getcwd函数，这个函数会造成很多的lstat和fstat系统调用。我们在NetBSD下运行了同样的测试，发现系统调用增加比较少，因为这个系统实现了一个getcwd的系统调用。

![图11](http://i4.tietuku.com/9ab5a1cc78aaa270.png)

　　第二个测试测量了一个网络爬虫从本地网站服务器下载文件。爬虫下载了大约140个文件，每秒有2mb的吞吐量。这个测试中，Systrace所花费的时间仅增加了5.2%。

　　Systrace所造成的性能影响，尽管可以看到，但是不会令人望而却步。尤其是对于一些交互性的网络应用，对于终端用户基本上没有可见的性能损失。

### 8 进一步的工作
　　这项工作为今后的研究提供了很多参考。Systrace可以被用来给应用程序制造随机的故障。调和允许我们引入错误的条件去观察应用程序怎么应对它。比如，我们可以模仿空间不够或者内存不够的情况。通过系统调用参数替换，我们可以改变应用程序和操作系统交互的方式。通过改变文件参数，我们有可能制造出一个在应用程序看来的虚拟的文件系统。我们可以重写应用程序访问的地址。这允许我们将网络流量重定向到不太懂的主机从而时间一个应用层的防火墙。

### 9 总结
　　本文提出了一种新的软件限制的工具。这个工具支持自动和交互式的策略生成，审计，入侵检测，权限提升，能够用在系统服务和用户软件上。我们认为，系统调用干涉是一种灵活且适当的防止入侵的机制。我们实现的Systrace能够在保障软件安全运行的同时具有良好的性能和优秀的可移植性。本文所解决的一些重要的问题没有被先前的研究解决过。把系统调用参数转换为可读的字符串能够让我们设计出一个简单的策略语言，这也使我们的系统能够自动和交互式地生成良好的规则。由此产生的限制规则，也不会影响应用程序原本的功能。

　　通过特权提升，我们可以减少应用程度原本的权限。我们可以给应用程序更细粒度的特权，应用程序也不需要请求root权限即可运行。这样，应用程序就不需要一直处在root状态，会根据给定的策略在需要的时候获得特权。

　　我们的安全分析讨论了我们怎么去解决系统调用干涉原本的问题，以及为什么我们的设计允许进一步的功能，比如入侵检测和预防。

　　我们分析了Systrace的性能，得出它的开销是可接受的，使用沙箱的用户不会感受到沙箱的存在。

### 10 致谢
　　我想要感谢Peter Honeyman, Terrence Kelly, Chuck Lever, Ken MacInnis, Joe McClain, Perry Metzger 和 Jose Nazario认真的审阅。我也感谢 Marius Eriksen, Angelos Keromytis, Patrick Mc-Daniel, Perry Metzger, Dug Song 和 Markus Watts在这个主题上和我讨论，给了我很多帮助。

　　
