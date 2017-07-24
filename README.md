# blog_samples_timing_2017

Samples for several my blog posts on performance measurements

Mainly in Ukrainian.

- Для компідяції PCM -- скрипт build_pcm_test.sh 
- Для всього решта -- CMakeLists.txt 

Включає наступні програми:
- boost_clocks.cpp  -- таймери boost
- browse_perf_counters_win32.cpp -- трішки модифікований приклад із MSDN для роботи із Windows performance counters і їх браузером. (Очевидно, ліцензія FreeBSD його не стосується).
- check_timeouts.cpp -- перевіряє різні способи заміряти час для різних платформ. 
- diagnose_sys.cpp -- виводиться деяка базова інформація про таймери.
- papi_test.cpp  -- мінімалістичний і трохи збочений тест PAPI. Так як кількість апаратних лічильників обмежена, (при чому, взнати, скільки лічильників потрібно конкретному виміру, не так просто), щоб не возитися із мультиплексуванням, за кожного запуску випадковим чином вибирається декілька вимірів. Якщо апаратних лічильників для них вистачило, вони виконуються. 
- pcm_test.cpp  -- простий тест [PCM](https://github.com/opcm/pcm)
- perf_counters_sample_win32.cpp  -- приклад використання Windows performance counters.
- run_metrics.cpp  -- різноманітні тести.