![d_FA6e9yde](https://github.com/user-attachments/assets/4d3f0ec7-3ce6-4ee0-b278-e763191927d8)# AVR-Dino-game

<p align="right">B915093 손주영</p>

# 소개
---
이 프로젝트는 크롬 브라우저에 포함된 게임으로 유명한 "공룡 게임"을 AVR에서 구현한 프로젝트입니다.
저는 게임을 만드는 것을 좋아하여 이전에 C#을 이용하여 유니티 엔진으로 게임을 만들어본 경험이 있습니다. 이 경험을 살려 강의에서 배운 내용을 기반으로 PC가 아닌 AVR이라는 새로운 환경에서 게임을 구현해보는 도전을 해보고 싶었습니다.

### 과목 연결성
---
이 프로젝트는 강의에서 배운 내용 중
- 세븐 세그먼트
- 외부 인터럽트(푸시버튼)
- 오버플로우 타이머(스피커 소리 출력 및 조절)
를 사용하였습니다.

### 기타 사항
---
이 프로젝트는 게임을 제작하는 프로젝트이므로, 구현에 객체지향 개발 환경이 더 적합하다 판단되어 C가 아닌 C++에서 만들었습니다.
기능 별로 클래스를 만들고 파일을 분할하여 여러개의 파일로 구성하였습니다.

# 기술적 배경
---
### 세븐 세그먼트
---

![Pasted image 20231021234400](https://github.com/user-attachments/assets/c0c8f0f1-86bf-43df-aba9-97b5e7cd3474)


적용: 공룡, 장애물과 게임오버 시 점수를 표시하는데 사용하였습니다.
### 인터럽트
---
인터럽트는 프로세서가 현재 작업 중이던 작업을 중단하고 다른 작업을 먼저 처리하도록 하는 역할을 합니다.
- SREG(Status Register): 전역 인터럽트를 활성화하는데 필요한 레지스터입니다.
- ISR (Interrupt Service Routine): 특정 인터럽트 발생 시 호출됩니다.
##### 외부 인터럽트(푸시버튼)
- EIMSK (External Interrupt Mask Register): 외부 인터럽트 활성화 여부를 정하는 레지스터 입니다.
- EICR (External Interrupt Control Register): 외부 인터럽트를 작동 시키는 조건(Rising, Falling 등)을 정하는 레지스터 입니다. 
- INTN\_vect: 특정 외부 인터럽트의 벡터 입니다. N에는 인터럽트 핀 번호가 들어갑니다.
적용: 외부 인터럽트의 공룡의 점프 버튼을 만들었습니다. 길게 누르면 더 오래 뛰도록 버튼을 누르고 땔 때 마다 EICR을 변경하여 더 섬세한 조작을 구현하였습니다.
##### 오버플로우 타이머
- OVF(Overflow Interrupt): AVR 내부 타이머/카운터(TCNT)가 증가하다가 최대값에서 하나 더 증가하는 오버플로우가 발생하면 실행되는 인터럽트 입니다.
- TCCR: 타이머/카운터가 동작하는 모드를 설정합니다. 
	- Normal: TCNT가 0에서 최대값(OVF)에 도달하면 초기화. 고정 주기
	- CTC: TCNT가 OCR이 되면 하드웨어(정확함)에서 초기화. 변동 주기
	- PWM: OCR에서 HIGH/LOW가 바뀐다. 고정 주기
	
적용: 오버플로우 타이머 2개를 사용하여 하나는 주파수, 나머지 하나는 음의 길이를 제어하여 게임 진행과 비동기적으로 작동하는 음향 효과를 구현하였습니다.  
### 동적 할당 불가
---
C++에서는 'new', 'delete'로 동적 메모리 할당을 지원하나, AVR 환경에서는 메모리 활용이 열악하여 이를 지원하지 않습니다. 때문에 구현에 있어 클래스로 변수, 메서드를 상속받거나 인스턴스를 생성하는데 제약이 있었습니다.
	
적용: 공룡, 장애물과 같은 오브젝트를 인스턴스 대신 스태틱 함수와 변수를 가진 클래스로 구현하였습니다.

# 개발 과정
---
### 프로그램 구상
- 2번째 세그먼트에 플레이어가 조작할 수 있는 공룡이 표시된다.
- 공룡은 버튼을 눌러 점프를 할 수 있고, 점프를 하면 점프하기 전 보다 한 칸 위에 표시된다.
- 누른 버튼을 떼거나 최대 체공시간 이상 공중에 있으면 다시 땅으로 내려온다. 
- 4번째 세그먼트에서 장애물이 주기적으로 나타나며, 한 칸씩 왼쪽으로 다가온다.
- 점프를 하면 장애물을 넘어갈 수 있고, 점수가 1점 추가된다.
- 지상에 있는 상태로 다가오는 장애물과 부딪히면 게임이 종료된다.
- 게임이 종료되면 점수가 출력 된다.

### 프로그램 구조 설계
---
![Uploadin<svg aria-roledescription="flowchart-v2" role="graphics-document document" viewBox="-8 -8 783.4375 489" height="489" xmlns="http://www.w3.org/2000/svg" width="783.4375" id="tmpMermaidElement717"><style>svg {background: white;}#tmpMermaidElement717{font-family:"trebuchet ms",verdana,arial,sans-serif;font-size:16px;fill:#333;}#tmpMermaidElement717 .error-icon{fill:#552222;}#tmpMermaidElement717 .error-text{fill:#552222;stroke:#552222;}#tmpMermaidElement717 .edge-thickness-normal{stroke-width:2px;}#tmpMermaidElement717 .edge-thickness-thick{stroke-width:3.5px;}#tmpMermaidElement717 .edge-pattern-solid{stroke-dasharray:0;}#tmpMermaidElement717 .edge-pattern-dashed{stroke-dasharray:3;}#tmpMermaidElement717 .edge-pattern-dotted{stroke-dasharray:2;}#tmpMermaidElement717 .marker{fill:#333333;stroke:#333333;}#tmpMermaidElement717 .marker.cross{stroke:#333333;}#tmpMermaidElement717 svg{font-family:"trebuchet ms",verdana,arial,sans-serif;font-size:16px;}#tmpMermaidElement717 .label{font-family:"trebuchet ms",verdana,arial,sans-serif;color:#333;}#tmpMermaidElement717 .cluster-label text{fill:#333;}#tmpMermaidElement717 .cluster-label span,#tmpMermaidElement717 p{color:#333;}#tmpMermaidElement717 .label text,#tmpMermaidElement717 span,#tmpMermaidElement717 p{fill:#333;color:#333;}#tmpMermaidElement717 .node rect,#tmpMermaidElement717 .node circle,#tmpMermaidElement717 .node ellipse,#tmpMermaidElement717 .node polygon,#tmpMermaidElement717 .node path{fill:#ECECFF;stroke:#9370DB;stroke-width:1px;}#tmpMermaidElement717 .flowchart-label text{text-anchor:middle;}#tmpMermaidElement717 .node .label{text-align:center;}#tmpMermaidElement717 .node.clickable{cursor:pointer;}#tmpMermaidElement717 .arrowheadPath{fill:#333333;}#tmpMermaidElement717 .edgePath .path{stroke:#333333;stroke-width:2.0px;}#tmpMermaidElement717 .flowchart-link{stroke:#333333;fill:none;}#tmpMermaidElement717 .edgeLabel{background-color:#e8e8e8;text-align:center;}#tmpMermaidElement717 .edgeLabel rect{opacity:0.5;background-color:#e8e8e8;fill:#e8e8e8;}#tmpMermaidElement717 .labelBkg{background-color:rgba(232, 232, 232, 0.5);}#tmpMermaidElement717 .cluster rect{fill:#ffffde;stroke:#aaaa33;stroke-width:1px;}#tmpMermaidElement717 .cluster text{fill:#333;}#tmpMermaidElement717 .cluster span,#tmpMermaidElement717 p{color:#333;}#tmpMermaidElement717 div.mermaidTooltip{position:absolute;text-align:center;max-width:200px;padding:2px;font-family:"trebuchet ms",verdana,arial,sans-serif;font-size:12px;background:hsl(80, 100%, 96.2745098039%);border:1px solid #aaaa33;border-radius:2px;pointer-events:none;z-index:100;}#tmpMermaidElement717 .flowchartTitleText{text-anchor:middle;font-size:18px;fill:#333;}#tmpMermaidElement717 :root{--mermaid-font-family:"trebuchet ms",verdana,arial,sans-serif;}</style><g><marker orient="auto" markerHeight="12" markerWidth="12" markerUnits="userSpaceOnUse" refY="5" refX="6" viewBox="0 0 10 10" class="marker flowchart" id="tmpMermaidElement717_flowchart-pointEnd"><path style="stroke-width: 1; stroke-dasharray: 1, 0;" class="arrowMarkerPath" d="M 0 0 L 10 5 L 0 10 z"></path></marker><marker orient="auto" markerHeight="12" markerWidth="12" markerUnits="userSpaceOnUse" refY="5" refX="4.5" viewBox="0 0 10 10" class="marker flowchart" id="tmpMermaidElement717_flowchart-pointStart"><path style="stroke-width: 1; stroke-dasharray: 1, 0;" class="arrowMarkerPath" d="M 0 5 L 10 10 L 10 0 z"></path></marker><marker orient="auto" markerHeight="11" markerWidth="11" markerUnits="userSpaceOnUse" refY="5" refX="11" viewBox="0 0 10 10" class="marker flowchart" id="tmpMermaidElement717_flowchart-circleEnd"><circle style="stroke-width: 1; stroke-dasharray: 1, 0;" class="arrowMarkerPath" r="5" cy="5" cx="5"></circle></marker><marker orient="auto" markerHeight="11" markerWidth="11" markerUnits="userSpaceOnUse" refY="5" refX="-1" viewBox="0 0 10 10" class="marker flowchart" id="tmpMermaidElement717_flowchart-circleStart"><circle style="stroke-width: 1; stroke-dasharray: 1, 0;" class="arrowMarkerPath" r="5" cy="5" cx="5"></circle></marker><marker orient="auto" markerHeight="11" markerWidth="11" markerUnits="userSpaceOnUse" refY="5.2" refX="12" viewBox="0 0 11 11" class="marker cross flowchart" id="tmpMermaidElement717_flowchart-crossEnd"><path style="stroke-width: 2; stroke-dasharray: 1, 0;" class="arrowMarkerPath" d="M 1,1 l 9,9 M 10,1 l -9,9"></path></marker><marker orient="auto" markerHeight="11" markerWidth="11" markerUnits="userSpaceOnUse" refY="5.2" refX="-1" viewBox="0 0 11 11" class="marker cross flowchart" id="tmpMermaidElement717_flowchart-crossStart"><path style="stroke-width: 2; stroke-dasharray: 1, 0;" class="arrowMarkerPath" d="M 1,1 l 9,9 M 10,1 l -9,9"></path></marker><g class="root"><g class="clusters"></g><g class="edgePaths"><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-A LE-B" id="L-A-B-0" d="M287.438,34.478L276.962,40.899C266.487,47.319,245.536,60.159,235.061,75.663C224.586,91.167,224.586,109.333,224.586,127.5C224.586,145.667,224.586,163.833,237.881,179.53C251.176,195.227,277.766,208.454,291.061,215.067L304.356,221.68"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-A LE-C" id="L-A-C-0" d="M287.438,29.781L270.792,36.984C254.146,44.188,220.854,58.594,204.208,74.88C187.563,91.167,187.563,109.333,187.563,127.5C187.563,145.667,187.563,163.833,187.563,182C187.563,200.167,187.563,218.333,187.563,236.5C187.563,254.667,187.563,272.833,203.971,288.15C220.38,303.466,253.197,315.931,269.606,322.164L286.014,328.397"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-A LE-D" id="L-A-D-0" d="M287.438,27.218L264.621,34.849C241.805,42.479,196.172,57.739,173.355,74.453C150.539,91.167,150.539,109.333,150.539,127.5C150.539,145.667,150.539,163.833,150.539,182C150.539,200.167,150.539,218.333,150.539,236.5C150.539,254.667,150.539,272.833,150.539,291C150.539,309.167,150.539,327.333,150.539,345.5C150.539,363.667,150.539,381.833,210.855,398.532C271.17,415.231,391.802,430.461,452.117,438.076L512.433,445.692"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-A LE-E" id="L-A-E-0" d="M319.459,37L321.389,43C323.319,49,327.179,61,329.109,72.117C331.039,83.233,331.039,93.467,331.039,98.583L331.039,103.7"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-A LE-F" id="L-A-F-0" d="M339.578,25.423L369.438,33.353C399.297,41.282,459.016,57.141,488.875,70.187C518.734,83.233,518.734,93.467,518.734,98.583L518.734,103.7"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-A LE-G" id="L-A-G-0" d="M339.578,22.152L400.079,30.626C460.581,39.101,581.583,56.051,642.085,73.609C702.586,91.167,702.586,109.333,702.586,127.5C702.586,145.667,702.586,163.833,702.586,182C702.586,200.167,702.586,218.333,702.586,236.5C702.586,254.667,702.586,272.833,702.586,287.033C702.586,301.233,702.586,311.467,702.586,316.583L702.586,321.7"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-B LE-H" id="L-B-H-0" d="M309.102,241.372L266.581,249.644C224.06,257.915,139.018,274.457,96.497,291.812C53.977,309.167,53.977,327.333,53.977,345.5C53.977,363.667,53.977,381.833,53.977,396.033C53.977,410.233,53.977,420.467,53.977,425.583L53.977,430.7"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-C LE-H" id="L-C-H-0" d="M290.969,355.046L259.517,362.538C228.065,370.03,165.161,385.015,128.98,397.846C92.799,410.678,83.339,421.355,78.61,426.694L73.88,432.033"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-B LE-C" id="L-B-C-0" d="M334.148,255L334.148,261C334.148,267,334.148,279,333.856,290.118C333.564,301.236,332.98,311.472,332.688,316.591L332.396,321.709"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-B LE-D" id="L-B-D-0" d="M359.195,242.447L393.277,250.539C427.359,258.631,495.523,274.816,529.605,291.991C563.688,309.167,563.688,327.333,563.688,345.5C563.688,363.667,563.688,381.833,565.441,396.08C567.195,410.327,570.703,420.654,572.457,425.818L574.211,430.982"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-C LE-D" id="L-C-D-0" d="M371.109,353.598L409.376,361.332C447.643,369.065,524.177,384.533,560.69,397.43C597.203,410.327,593.695,420.654,591.941,425.818L590.188,430.982"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-E LE-B" id="L-E-B-0" d="M331.039,146L331.039,152C331.039,158,331.039,170,331.331,181.118C331.623,192.236,332.207,202.472,332.499,207.591L332.791,212.709"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-F LE-B" id="L-F-B-0" d="M500.169,146L494.148,152C488.127,158,476.085,170,453.404,182.99C430.723,195.98,397.403,209.96,380.743,216.95L364.083,223.941"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-F LE-C" id="L-F-C-0" d="M527.205,146L529.952,152C532.699,158,538.193,170,540.94,185.083C543.688,200.167,543.688,218.333,543.688,236.5C543.688,254.667,543.688,272.833,515.78,289.069C487.873,305.305,432.058,319.61,404.151,326.762L376.243,333.914"></path><path marker-end="url(#tmpMermaidElement717_flowchart-pointEnd)" style="fill:none;" class="edge-thickness-normal edge-pattern-solid flowchart-link LS-G LE-D" id="L-G-D-0" d="M702.586,364L702.586,370C702.586,376,702.586,388,690.137,399.636C677.688,411.271,652.79,422.543,640.342,428.179L627.893,433.814"></path></g><g class="edgeLabels"><g transform="translate(224.5859375, 127.5)" class="edgeLabel"><g transform="translate(-17.0234375, -11)" class="label"><foreignObject height="22" width="34.046875"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">uses</span></div></foreignObject></g></g><g transform="translate(187.5625, 182)" class="edgeLabel"><g transform="translate(-17.0234375, -11)" class="label"><foreignObject height="22" width="34.046875"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">uses</span></div></foreignObject></g></g><g transform="translate(150.5390625, 236.5)" class="edgeLabel"><g transform="translate(-17.0234375, -11)" class="label"><foreignObject height="22" width="34.046875"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">uses</span></div></foreignObject></g></g><g transform="translate(331.0390625, 73)" class="edgeLabel"><g transform="translate(-33.4609375, -11)" class="label"><foreignObject height="22" width="66.921875"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">interrupt</span></div></foreignObject></g></g><g transform="translate(518.734375, 73)" class="edgeLabel"><g transform="translate(-38.78125, -11)" class="label"><foreignObject height="22" width="77.5625"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">outputs to</span></div></foreignObject></g></g><g transform="translate(702.5859375, 182)" class="edgeLabel"><g transform="translate(-38.78125, -11)" class="label"><foreignObject height="22" width="77.5625"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">outputs to</span></div></foreignObject></g></g><g transform="translate(53.9765625, 345.5)" class="edgeLabel"><g transform="translate(-28.28125, -11)" class="label"><foreignObject height="22" width="56.5625"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">inherits</span></div></foreignObject></g></g><g transform="translate(102.2578125, 400)" class="edgeLabel"><g transform="translate(-28.28125, -11)" class="label"><foreignObject height="22" width="56.5625"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">inherits</span></div></foreignObject></g></g><g transform="translate(334.1484375, 291)" class="edgeLabel"><g transform="translate(-50.5390625, -11)" class="label"><foreignObject height="22" width="101.078125"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">interacts with</span></div></foreignObject></g></g><g transform="translate(563.6875, 345.5)" class="edgeLabel"><g transform="translate(-17.0234375, -11)" class="label"><foreignObject height="22" width="34.046875"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">uses</span></div></foreignObject></g></g><g transform="translate(600.7109375, 400)" class="edgeLabel"><g transform="translate(-17.0234375, -11)" class="label"><foreignObject height="22" width="34.046875"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">uses</span></div></foreignObject></g></g><g transform="translate(331.0390625, 182)" class="edgeLabel"><g transform="translate(-28.40625, -11)" class="label"><foreignObject height="22" width="56.8125"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">triggers</span></div></foreignObject></g></g><g transform="translate(464.04296875, 182)" class="edgeLabel"><g transform="translate(-29.90625, -11)" class="label"><foreignObject height="22" width="59.8125"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">displays</span></div></foreignObject></g></g><g transform="translate(543.6875, 236.5)" class="edgeLabel"><g transform="translate(-29.90625, -11)" class="label"><foreignObject height="22" width="59.8125"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">displays</span></div></foreignObject></g></g><g transform="translate(702.5859375, 400)" class="edgeLabel"><g transform="translate(-64.8515625, -11)" class="label"><foreignObject height="22" width="129.703125"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="edgeLabel">plays sound from</span></div></foreignObject></g></g></g><g class="nodes"><g transform="translate(313.5078125, 18.5)" id="flowchart-A-13470" class="node default default flowchart-label"><rect height="37" width="52.140625" y="-18.5" x="-26.0703125" ry="0" rx="0" style="fill:#98FB98;" class="basic label-container"></rect><g transform="translate(-18.5703125, -11)" style="" class="label"><rect></rect><foreignObject height="22" width="37.140625"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="nodeLabel">Main</span></div></foreignObject></g></g><g transform="translate(334.1484375, 236.5)" id="flowchart-B-13471" class="node default default flowchart-label"><rect height="37" width="50.09375" y="-18.5" x="-25.046875" ry="0" rx="0" style="fill:#98FB98;" class="basic label-container"></rect><g transform="translate(-17.546875, -11)" style="" class="label"><rect></rect><foreignObject height="22" width="35.09375"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="nodeLabel">Dino</span></div></foreignObject></g></g><g transform="translate(331.0390625, 345.5)" id="flowchart-C-13473" class="node default default flowchart-label"><rect height="37" width="80.140625" y="-18.5" x="-40.0703125" ry="0" rx="0" style="fill:#98FB98;" class="basic label-container"></rect><g transform="translate(-32.5703125, -11)" style="" class="label"><rect></rect><foreignObject height="22" width="65.140625"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="nodeLabel">Obstacle</span></div></foreignObject></g></g><g transform="translate(582.19921875, 454.5)" id="flowchart-D-13475" class="node default default flowchart-label"><rect height="37" width="129.015625" y="-18.5" x="-64.5078125" ry="0" rx="0" style="fill:#98FB98;" class="basic label-container"></rect><g transform="translate(-57.0078125, -11)" style="" class="label"><rect></rect><foreignObject height="22" width="114.015625"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="nodeLabel">SoundManager</span></div></foreignObject></g></g><g transform="translate(331.0390625, 127.5)" id="flowchart-E-13477" class="node default default flowchart-label"><rect height="37" width="108.859375" y="-18.5" x="-54.4296875" ry="0" rx="0" style="fill:#ADFF2F;" class="basic label-container"></rect><g transform="translate(-46.9296875, -11)" style="" class="label"><rect></rect><foreignObject height="22" width="93.859375"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="nodeLabel">Jump Button</span></div></foreignObject></g></g><g transform="translate(518.734375, 127.5)" id="flowchart-F-13479" class="node default default flowchart-label"><rect height="37" width="166.53125" y="-18.5" x="-83.265625" ry="0" rx="0" style="fill:#FFD700;" class="basic label-container"></rect><g transform="translate(-75.765625, -11)" style="" class="label"><rect></rect><foreignObject height="22" width="151.53125"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="nodeLabel">4 7-Segment Display</span></div></foreignObject></g></g><g transform="translate(702.5859375, 345.5)" id="flowchart-G-13481" class="node default default flowchart-label"><rect height="37" width="75.375" y="-18.5" x="-37.6875" ry="0" rx="0" style="fill:#FFD700;" class="basic label-container"></rect><g transform="translate(-30.1875, -11)" style="" class="label"><rect></rect><foreignObject height="22" width="60.375"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="nodeLabel">Speaker</span></div></foreignObject></g></g><g transform="translate(53.9765625, 454.5)" id="flowchart-H-13483" class="node default default flowchart-label"><rect height="37" width="107.953125" y="-18.5" x="-53.9765625" ry="0" rx="0" style="fill:#E0FFFF;" class="basic label-container"></rect><g transform="translate(-46.4765625, -11)" style="" class="label"><rect></rect><foreignObject height="22" width="92.953125"><div style="display: inline-block; white-space: nowrap;" xmlns="http://www.w3.org/1999/xhtml"><span class="nodeLabel">GameObject</span></div></foreignObject></g></g></g></g></g></svg>g d_FA6e9yde.svg…]()


- **Main**: 프로그램의 주 실행 파일입니다. 이는 게임의 다른 주요 컴포넌트들을 사용하며, 기능 초기화 및 게임 모드 설정을 통해 실행 흐름을 제어합니다.
    
- **Dino**: 공룡 캐릭터를 나타내는 클래스입니다. 이는 "GameObject"로부터 상속받으며, 장애물과 상호작용하고 사운드 매니저를 사용합니다.
    
- **Obstacle**: 게임 내의 장애물을 나타내는 클래스입니다. 이 역시 "GameObject"로부터 상속받으며, 사운드 매니저를 사용합니다.
    
- **SoundManager**: 게임 내의 사운드를 관리하는 클래스입니다. 공룡과 장애물에 의해 사용되며, 스피커를 통해 소리를 출력합니다.
    
- **Jump Button**: 게임 내에서 공룡의 점프를 트리거하는 외부 인터럽트입니다. Main과 연결되어 있으며, Dino에 점프 명령을 전달합니다.
    
- **4 7-Segment Display & Speaker**: 게임의 시각적 및 청각적 출력을 담당하는 장치입니다. 세븐 세그먼트 디스플레이는 공룡과 장애물 정보 및 점수를 표시하고, 스피커는 사운드 매니저에서 오는 소리를 재생합니다.
	
- **GameObject**: 공룡과 장애물 클래스가 공통적으로 상속받는 베이스 클래스입니다. 게임 내의 기본적인 오브젝트 구조를 정의합니다.
# 구현 세부사항
---
### main.cpp

- 세븐 세그먼트
``` C++
void showCharacters(){
	unsigned char segment;
	for(int i = 0; i<4;i++){	//네 7세그먼트에서 각각 출력할 내용을 bitwise OR 연산으로 겹쳐 출력합니다.
		segment = 0;
		segment |= Dino::displaySeg(i);
		segment |= Obstacle::displaySeg(i);
		
		PORTC=~(1<<i);		//출력할 7세그먼트 옯기기
		
		PORTA = segment;	//출력
		
		_delay_ms(2);
	}	
};
```
세븐 세그먼트로 캐릭터를 출력하는 함수 입니다.

- 외부 인터럽트
```C++
ISR(INT0_vect){	//푸시 버튼을 누르면 호출됩니다. 
	if(gameMode == MAINGAME){
		if(PIND & 1){
			//버튼 누름
			EICRA &=0;	//버튼을 땔 때 인터럽트가 작동하도록 설정
			Dino::jump(); //점프 명령
			}else{
			//버튼 땜
			EICRA |=1;	//버튼을 누를 때 인터럽트가 작동하도록 설정
			Dino::land(); //착지 명령
		}
	}
}
```
버튼을 누르고 때는 것을 감지하기 위한 외부 인터럽트 처리 코드 입니다.

### SoundManager.cpp

- 오버플로우 타이머
```C++
ISR(TIMER0_OVF_vect){
	TCNT0 = SoundManager::Timer0Value;
	if (SoundManager::isSoundOn) {
		PORTF ^= 0x80; // isSoundOn이 true일 때만 토글
	}
}

ISR(TIMER2_OVF_vect) {
	SoundManager::noteTimeCounter++; // 음의 길이 카운팅

	if (SoundManager::noteTimeCounter == 15) {
		SoundManager::noteTimeCounter = 0; // 카운터 재설정

		if (SoundManager::noteCount  < 1) {
			// 다음 음 재생
			SoundManager::Timer0Value = 255-(1000000/(8*C1)); // 다음 음의 주파수 값 설정
			SoundManager::noteCount++; // 다음 음으로 넘어가기
		} else {
			// 모든 음 재생 완료
			SoundManager::isSoundOn = false; // 소리 재생 중지
			SoundManager::noteCount = 0; // 음 카운터 초기화
		}
	}
}
```
게임 진행 중 \_delay\_ms()를 실행하면 게임이 중간에 정지되어버리므로, 소리 출력을 2개의 타이머를 사용하여 구현하였습니다.

# 결론
---
### 최종 결과
버튼을 눌러 다가오는 장애물을 점프로 뛰어넘는 공룡 게임을 스피커의 효과음과 함께 7segment 디스플레이로 AVR에서 구현할 수 있었습니다. 게임의 시작과 끝이 명확하고, 다가오는 장애물의 속도가 게임이 진행될수록 점점 빨라지는 변화도 구현하여 게임을 즐기기에 문제가 없었습니다. 

### 성과 및 학습점
- AVR의 특징인 인터럽트, 카운터를 다양하게 사용해보는 도전적인 경험을 할 수 있었습니다. 외부 인터럽트 처리는 EICRA를 계속 변경하여 버튼의 누르고 때는 섬세한 조작을 구현했고, 카운터는 두 개를 동시에 사용하며 서로에게 영향을 주도록 만들었습니다.
- 학교에서 배운 파이썬, 독학한 C, 게임 개발을 위한 C#을 사용해본 적이 있어 객체지향과 알고리즘에 대한 지식과 경험은 있었으나, C++을 사용해 본 적은 없었습니다. 프로젝트 제작 기간 중 하루 정도를 PWM과 음정을 동시에 제어하는 프로젝트를 구현하려다 중단하고 새 프로젝트를 시작하여 일정 상 시간이 부족했고 사용할 언어도 바뀌었기에, 부족한 시간 동안 최대한의 생산성을 얻고자 Chat GPT의 도움을 받았습니다. C++ 문법에서 도움을 받았고, 자료 중간에 삽입된 그래프도 생성하였습니다. 덕분에 그래프를 그리는 시간을 아낄 수 있었고, 강의 외의 내용 중 프로젝트에 필요한 많은 것을 빠르게 배울 수 있었습니다.
### 개선점 및 향후 계획
- **개선할 점**: 당장 게임을 즐기는 데는 문제가 없었으나, 7세그먼트를 출력할 때 사용한 delay\_ms\_()를 또 다른 타이머 오버플로우 인터럽트를 사용하여 비동기식으로 처리한다면 좀 더 깔끔한 소리와 출력을 얻을 수 있을 것으로 기대됩니다.
- **향후 계획**: 저번 여름방학에 아두이노를 활용한 프로젝트를 진행한 적이 있었으나, 임베디드에 익숙하지 않아 어려움을 겪은 경험이 있었습니다. 다음에 아두이노나 AVR과 같은 임베디드 시스템을 사용하는 프로젝트를 진행하게 된다면, 이번 강의와 프로젝트에서 배운 내용을 바탕으로 C++, 비동기식 프로그래밍, 외부 인터럽트, PWM등 다양한 기법을 효율적으로 사용하여 더 완성도 높고 즐거운 개발을 할 수 있을 것 같습니다.
