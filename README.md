# N-AI-Spam
Non-AI Spam detector is an advanced heuristic-based spam detection algorithm written in C with a measured performance of F1 = 95.63% on 2000’s emails. The program won the Bitdefender Baliza 2022 challenge.

<p>Some of the heuristics used include:</p>
                <ul>
                    <li><b>Punctuation</b> could indicate a SPAM email - for example, dollar signs could indicate money transfers wanted by potential malicious users, while exclamation marks could indicate urgency. A score is determined based on the number of characters relative to the number of words counted in the email;</li>
                    <li><b>Uppercase</b> letters are usually used in excess or not at all when it comes to SPAM email. <a href=\"https://www.desmos.com/calculator/lw4qgluitr\">Here</a> is the formula I found best suited for computing this score. In the end, everything is converted relative to the toal count of non-space characters;</li>
                    <li><b>Consonants</b> don't appear naturally in groups of 4 or more, so this clue could raise a red flag. Obviously, links should be ignored;</li>
                    <li><b>Keywords</b> that trigger a SPAM are very important - they include words such as 'money', 'purchase', 'deposit', 'diamond', 'risk', 'bank' etc. However, there are some words that could lower this score, including 'forwarded', 'newsletter' and 'yahoo' (it was big back then). If the same word occures multiple times, say n times in the same email, it should be counted only about sqrt(n) times (my formula is a little bit more complicated, but for small values this works too);</li>
                    <li><b>Known spam email addresses</b> should also be taken into account.</li>
                </ul>

## License

The code is availabe under the MIT license.
