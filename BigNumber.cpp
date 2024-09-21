#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <string>

class BigNumber
{
private:
    // Vector to store the digits of the number in reverse order (least significant digit first)
    std::vector<int> number_digits;
    bool isNegative = false;

public:
    // Default constructor to initialize BigNumber to zero
    BigNumber()
    {
        number_digits.push_back(0);
        isNegative = false;
    }

    // Constructor from long long
    BigNumber(long long num)
    {
        if (num == 0)
        {
            number_digits.push_back(0);
            isNegative = false;
            return;
        }

        isNegative = (num < 0);
        num = std::abs(num);

        while (num > 0)
        {
            number_digits.push_back(num % 10);
            num /= 10;
        }
    }

    // Constructor to initialize BigNumber from a string
    BigNumber(const std::string &number)
    {
        if (number.empty())
        {
            number_digits.push_back(0);
            isNegative = false;
            return;
        }

        size_t start = 0; // Start position for reading digits

        // Check for sign
        if (number[0] == '-')
        {
            isNegative = true;
            start = 1; // Start after the negative sign
        }
        else if (number[0] == '+')
        {
            start = 1; // Start after the positive sign
        }

        // Read the string in reverse order to fill digits
        number_digits.clear();
        for (size_t i = number.length(); i > start; --i)
        {
            char c = number[i - 1];
            if (!std::isdigit(c))
            {
                throw std::invalid_argument("Invalid character in input string");
            }
            number_digits.push_back(c - '0');
        }

        trimLeadingZeros();
    }

    // Remove leading zeros from the number
    void trimLeadingZeros()
    {
        while (number_digits.size() > 1 && number_digits.back() == 0)
        {
            number_digits.pop_back();
        }
        if (number_digits.size() == 1 && number_digits[0] == 0)
        {
            isNegative = false; // Zero is non-negative
        }
    }

    // absoulte value of the number
    BigNumber absolute() const
    {
        BigNumber result = *this;
        result.isNegative = false;
        return result;
    }

    // Convert the number to string
    std::string to_string() const
    {
        // If the BigNum represents zero, return "0"
        if (number_digits.size() == 1 && number_digits[0] == 0)
        {
            return "0";
        }

        std::string result;

        // Add negative sign if the number is negative
        if (isNegative)
        {
            result += '-';
        }

        // Iterate through the digits in reverse order
        for (auto it = number_digits.rbegin(); it != number_digits.rend(); ++it)
        {
            result += std::to_string(*it); // Append each digit as a character
        }

        return result;
    }

    // overloading == operator
    bool operator==(const BigNumber &other_number) const
    {
        return number_digits == other_number.number_digits && isNegative == other_number.isNegative;
    }

    // overloading != operator
    bool operator!=(const BigNumber &other_number) const
    {
        return !(*this == other_number);
    }

    // overloading < operator
    bool operator<(const BigNumber &other_number) const
    {
        if (isNegative != other_number.isNegative)
        {
            return isNegative;
        }

        if (number_digits.size() != other_number.number_digits.size())
        {
            return (isNegative ? number_digits.size() > other_number.number_digits.size() : number_digits.size() < other_number.number_digits.size());
        }

        for (int i = number_digits.size() - 1; i >= 0; i--)
        {
            if (number_digits[i] != other_number.number_digits[i])
            {
                return (isNegative ? number_digits[i] > other_number.number_digits[i] : number_digits[i] < other_number.number_digits[i]);
            }
        }

        return false;
    }

    // overloading > operator
    bool operator>(const BigNumber &other_number) const
    {
        return other_number < *this;
    }

    // overloading <= operator
    bool operator<=(const BigNumber &other_number) const
    {
        return !(*this > other_number);
    }

    // overloading >= operator
    bool operator>=(const BigNumber &other_number) const
    {
        return !(*this < other_number);
    }

    // Helper function to add absolute values of two BigNumbers
    BigNumber addAbsoluteValues(const BigNumber &a, const BigNumber &b) const
    {
        BigNumber result;
        result.isNegative = false;

        int carry = 0;
        size_t max_size = std::max(a.number_digits.size(), b.number_digits.size());
        result.number_digits.clear(); // Clear the result vector

        for (size_t i = 0; i < max_size || carry != 0; ++i)
        {
            int digit_sum = carry;
            if (i < a.number_digits.size())
                digit_sum += a.number_digits[i];
            if (i < b.number_digits.size())
                digit_sum += b.number_digits[i];

            result.number_digits.push_back(digit_sum % 10);
            carry = digit_sum / 10;
        }

        result.trimLeadingZeros();
        return result;
    }

    // Helper function to subtract absolute values of two BigNumbers (a - b) assuming |a| >= |b|
    BigNumber subtractAbsoluteValues(const BigNumber &a, const BigNumber &b) const
    {
        BigNumber result;
        result.isNegative = false;

        int borrow = 0;
        result.number_digits.clear(); // Clear the result vector

        for (size_t i = 0; i < a.number_digits.size(); ++i)
        {
            int digit_diff = a.number_digits[i] - borrow;
            if (i < b.number_digits.size())
            {
                digit_diff -= b.number_digits[i];
            }

            if (digit_diff < 0)
            {
                digit_diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }

            result.number_digits.push_back(digit_diff);
        }

        result.trimLeadingZeros();
        return result;
    }

    // Overload the + operator for BigNumber
    BigNumber operator+(const BigNumber &other_number) const
    {
        if (isNegative == other_number.isNegative)
        {
            // If both numbers have the same sign, add their absolute values
            BigNumber result = addAbsoluteValues(*this, other_number);
            result.isNegative = isNegative; // The result will have the same sign as both numbers
            return result;
        }
        else
        {
            // If the signs are different, subtract the smaller absolute value from the larger
            if (absolute() >= other_number.absolute())
            {
                BigNumber result = subtractAbsoluteValues(*this, other_number);
                result.isNegative = isNegative; // Keep the sign of the larger absolute value
                return result;
            }
            else
            {
                BigNumber result = subtractAbsoluteValues(other_number, *this);
                result.isNegative = other_number.isNegative; // Keep the sign of the larger absolute value
                return result;
            }
        }
    }

    // Overload the - operator for BigNumber
    BigNumber operator-(const BigNumber &other_number) const
    {
        // Subtraction is addition with the opposite sign
        BigNumber negated_other = other_number;
        negated_other.isNegative = !other_number.isNegative; // Flip the sign
        return *this + negated_other;
    }

    // Overload the * operator for BigNumber
    BigNumber operator*(const BigNumber &other_number) const
    {
        BigNumber result;
        result.number_digits.resize(number_digits.size() + other_number.number_digits.size(), 0); // Clear the result vector
        result.isNegative = (isNegative != other_number.isNegative); // The sign of the result is negative if the signs are different

        // Multiply the absolute values of the two numbers
        for (size_t i = 0; i < number_digits.size(); ++i)
        {
            for (size_t j = 0; j < other_number.number_digits.size(); ++j)
            {
                int product = number_digits[i] * other_number.number_digits[j];
                int position = i + j;
                while (product > 0)
                {
                    if (position == result.number_digits.size())
                    {
                        result.number_digits.push_back(0);
                    }
                    product += result.number_digits[position];
                    result.number_digits[position] = product % 10;
                    product /= 10;
                    position++;
                }
            }
        }

        result.trimLeadingZeros();
        return result;
    }

    // Overload the % operator for BigNumber (modulus operation)
    BigNumber operator%(const BigNumber &other_number) const
    {
        // Perform division and return the remainder
        return divide(other_number).second;
    }

    // Overload the / operator for BigNumber
    BigNumber operator/(const BigNumber &other_number) const
    {
        // Perform division and return the quotient
        return divide(other_number).first;
    }

    // Helper function to divide two BigNumbers
    std::pair<BigNumber, BigNumber> divide(const BigNumber &other_number) const
    {
        if (other_number == BigNumber("0"))
        {
            // throw std::invalid_argument("Division by zero");
            std::cout << "Does not exist" << std::endl;
            return {BigNumber("0"), BigNumber("0")};
        }

        BigNumber result, remainder;
        result.number_digits.resize(number_digits.size(), 0);
        result.isNegative = isNegative != other_number.isNegative;
        remainder.isNegative = isNegative;

        for (int i = number_digits.size() - 1; i >= 0; --i)
        {
            remainder.number_digits.insert(remainder.number_digits.begin(), number_digits[i]);
            remainder.trimLeadingZeros();

            int quotientDigit = 0;
            int left = 0, right = 10;
            while (left <= right)
            {
                int mid = (left + right) / 2;
                BigNumber candidate = other_number.absolute() * BigNumber(mid);
                if (candidate <= remainder.absolute())
                {
                    quotientDigit = mid;
                    left = mid + 1;
                }
                else
                {
                    right = mid - 1;
                }
            }

            BigNumber product = other_number.absolute() * BigNumber(quotientDigit);
            remainder = remainder - product;

            result.number_digits.insert(result.number_digits.begin(), quotientDigit);
        }

        result.trimLeadingZeros();
        remainder.trimLeadingZeros();
        return {result, remainder};
    }

    // Overload the unary - operator for BigNumber
    BigNumber operator-() const
    {
        BigNumber result = *this;
        result.isNegative = !isNegative;
        return result;
    }

    // function for modalar addition
    BigNumber modAddition(const BigNumber &other_number, const BigNumber &modulus)
    {
        BigNumber result = *this + other_number;
        return result % modulus;
    }

    // fuction for modular multiplication
    BigNumber modMultiplication(const BigNumber &other_number, const BigNumber &modulus)
    {
        BigNumber result = *this * other_number;
        return result % modulus;
    }

    BigNumber modInverse(const BigNumber &modulus)
    {
        BigNumber a = *this;
        BigNumber m = modulus;
        BigNumber m0 = m;
        BigNumber t, q;
        BigNumber x0 = 0, x1 = 1;

        if (m == 1)
            return 0;

        // Apply extended Euclid Algorithm
        while (a > 1)
        {
            // q is quotient
            q = a / m;

            t = m;

            // m is remainder now, process same as Euclid's algo
            m = a % m, a = t;

            t = x0;

            x0 = x1 - q * x0;

            x1 = t;
        }

        // Make x1 positive
        if (x1 < 0)
            x1 = x1 + m0;

        return x1;
    }
};

int main()
{
    
    BigNumber num0("0");
    BigNumber num1("7411983660145561438669578801503455163517312471829001959053178951383381181774990719242089448703004658285326780366787478508242288148487675488967829478156097");
    BigNumber num2("6792552932193534197244105339257986521498374343822704414426523273384151396718117732757475780861305360793165960264029209773235657982871742356434819199604765");
    BigNumber num3("21719959093543122193271597345913092765662363758920770284300105782111269136091634723077329996229524739293188973726960630197805213008905755855383760381155029228488818252788461905700483460905197645298493264714083598738790014487001568691651326578867087973323242769874902344568522847443006422922857740695323511933");
    BigNumber num4("87598250036046328361573979505897579787456709099174749148423254841095346166072716324373693401289806040349925933362124410478422643300338463347490089195912682110518980566451036360852877358396540284568447404069928935265327993407370576884662731932897796849068204195588940852651911426296135682164641491366577360037");
    BigNumber num5("6336461024454555932117034620426269405603205053829332619775501312678451912302336981110491969510120496437848174308143936942302152441755195312221302326928296671425622870962436283222537438995495994212112578469660192949932564280680664177812760498539627014299285708707719544379308855587018396967627039218836334123966973818724993636175084927568049941779992422611228286435776384882043784788594220553355992607686030148563073894634656307367612729620597238212220728445628077696393146609689625215720572867360334177879067717950179312553546441650724578725404169330430981346873295541384523278548267474572352974723334945650111500043");
    BigNumber num6("8449622564999510979567534763743016245490938299513553476367623548087314463825916726840358732482392644089893034659682191348187196141081688711202445010879993539338484782934159522781471451625278146742653056694035432282276816078300197792254456055759699493943356657434809391667642643820558132237952918124823097132768559236232937479990916309231969233436801659165812808849043501073137952450328982732824149765984464505632978656066724218121756956371844484919723384457896579832427535946654818374470626232217654436799112708022019051115293412561574522303002281961937351058670934976393414095015267279125706174486076352752867291324");
    BigNumber num7("-21719959093543122193271597345913092765662363758920770284300105782111269136091634723077329996229524739293188973726960630197805213008905755855383760381155029228488818252788461905700483460905197645298493264714083598738790014487001568691651326578867087973323242769874902344568522847443006422922857740695323511933");
    BigNumber num8("-6336461024454555932117034620426269405603205053829332619775501312678451912302336981110491969510120496437848174308143936942302152441755195312221302326928296671425622870962436283222537438995495994212112578469660192949932564280680664177812760498539627014299285708707719544379308855587018396967627039218836334123966973818724993636175084927568049941779992422611228286435776384882043784788594220553355992607686030148563073894634656307367612729620597238212220728445628077696393146609689625215720572867360334177879067717950179312553546441650724578725404169330430981346873295541384523278548267474572352974723334945650111500043");
    BigNumber num9("-87598250036046328361573979505897579787456709099174749148423254841095346166072716324373693401289806040349925933362124410478422643300338463347490089195912682110518980566451036360852877358396540284568447404069928935265327993407370576884662731932897796849068204195588940852651911426296135682164641491366577360037");
    BigNumber num10("1");
    BigNumber num11("6792552932193534197244105339257986521498374343822704414426523273384151396718117732757475780861305360793165960264029209773235657982871742356434819199604762");
    BigNumber num12("-7411983660145561438669578801503455163517312471829001959053178951383381181774990719242089448703004658285326780366787478508242288148487675488967829478156097");

    BigNumber mod_512("13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084096");
    BigNumber mod_1024("179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216");
    BigNumber mod_2048("32317006071311007300714876688669951960444102669715484032130345427524655138867890893197201411522913463688717960921898019494119559150490921095088152386448283120630877367300996091750197750389652106796057638384067568276792218642619756161838094338476170470581645852036305042887575891541065808607552399123930385521914333389668342420684974786564569494856176035326322058077805659331026192708460314150258592864177116725943603718461857357598351152301645904403697613233287231227125684710820209725157101726931323469678542580656697935045997268352998638215525166389437335543602135433229604645318478604952148193555853611059596230656");

    // Addtion test cases
    
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num1.to_string() << std::endl;
    std::cout << "Number 2: " << num0.to_string() << std::endl;
    std::cout << "modulus : 512bits" << std::endl;
    BigNumber sum1 = num1.modAddition(num0, mod_512);
    std::cout << "Calculated    : " << sum1.to_string() << std::endl;
    std::cout << "Expected      : 7411983660145561438669578801503455163517312471829001959053178951383381181774990719242089448703004658285326780366787478508242288148487675488967829478156097" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num1.to_string() << std::endl;
    std::cout << "Number 2: " << num2.to_string() << std::endl;
    std::cout << "modulus : 512bits" << std::endl;
    BigNumber sum2 = num1.modAddition(num2, mod_512);
    std::cout << "Calculated    : " << sum2.to_string() << std::endl;
    std::cout << "Expected      : 796728662396498536339659142555595557536320995059312995756140781045768548419561475197690931397406591388460882444330637427724063319412847898968999671676766" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num3.to_string() << std::endl;
    std::cout << "Number 2: " << num4.to_string() << std::endl;
    std::cout << "modulus : 1024bits" << std::endl;
    BigNumber sum3 = num3.modAddition(num4, mod_1024);
    std::cout << "Calculated    : " << sum3.to_string() << std::endl;
    std::cout << "Expected      : 109318209129589450554845576851810672553119072858095519432723360623206615302164351047451023397519330779643114907089085040676227856309244219202873849577067711339007798819239498266553360819301737929866940668784012534004118007894372145576314058511764884822391446965463843197220434273739142105087499232061900871970" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num5.to_string() << std::endl;
    std::cout << "Number 2: " << num6.to_string() << std::endl;
    std::cout << "modulus : 2048bits" << std::endl;
    BigNumber sum4 = num5.modAddition(num6, mod_2048);
    std::cout << "Calculated    : " << sum4.to_string() << std::endl;
    std::cout << "Expected      : 14786083589454066911684569384169285651094143353342886096143124860765766376128253707950850701992513140527741208967826128290489348582836884023423747337808290210764107653896595806004008890620774140954765635163695625232209380358980861970067216554299326508242642366142528936046951499407576529205579957343659431256735533054957931116166001236800019175216794081777041095284819885955181737238923203286180142373670494654196052550701380525489369685992441723131944112903524657528820682556344443590191199099577988614678180425972198363668839854212299101028406451292368332405544230517777937373563534753698059149209411298402978791367" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num7.to_string() << std::endl;
    std::cout << "Number 2: " << num4.to_string() << std::endl;
    std::cout << "modulus : 1024bits" << std::endl;
    BigNumber sum5 = num7.modAddition(num4, mod_1024);
    std::cout << "Calculated    : " << sum5.to_string() << std::endl;
    std::cout << "Expected      : 65878290942503206168302382159984487021794345340253978864123149058984077029981081601296363405060281301056736959635163780280617430291432707492106328814757652882030162313662574455152393897491342639269954139355845336526537978920369008193011405354030708875744961425714038508083388578853129259241783750671253848104" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num8.to_string() << std::endl;
    std::cout << "Number 2: " << num6.to_string() << std::endl;
    std::cout << "modulus : 2048bits" << std::endl;
    BigNumber sum6 = num8.modAddition(num6, mod_2048);
    std::cout << "Calculated    : " << sum6.to_string() << std::endl;
    std::cout << "Expected      : 2113161540544955047450500143316746839887733245684220856592122235408862551523579745729866762972272147652044860351538254405885043699326493398981142683951696867912861911971723239558934012629782152530540478224375239332344251797619533614441695557220072479644070948727089847288333788233539735270325878905986763008801585417507943843815831381663919291656809236554584522413267116191094167661734762179468157158298434357069904761432067910754144226751247246707502656012268502136034389336965193158750053364857320258920044990071839738561746970910849943577598112631506369711797639435008890816466999804553353199762741407102755791281" << std::endl;

    // Multiplication test cases

    std::cout << "===========================================================================================" << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num1.to_string() << std::endl;
    std::cout << "Number 2: " << num0.to_string() << std::endl;
    std::cout << "modulus : 512bits" << std::endl;
    BigNumber product1 = num1.modMultiplication(num0, mod_512);
    std::cout << "Calculated    : " << product1.to_string() << std::endl;
    std::cout << "Expected      : 0" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num1.to_string() << std::endl;
    std::cout << "Number 2: " << num2.to_string() << std::endl;
    std::cout << "modulus : 512bits" << std::endl;
    BigNumber product2 = num1.modMultiplication(num2, mod_512);
    std::cout << "Calculated    : " << product2.to_string() << std::endl;
    std::cout << "Expected      : 12515075797697995846956240208764939695950768703025659807810571944662006202778458249170429605928138870908686307763974182923128121001274469827172323354282589" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num3.to_string() << std::endl;
    std::cout << "Number 2: " << num4.to_string() << std::endl;
    std::cout << "modulus : 1024bits" << std::endl;
    BigNumber product3 = num3.modMultiplication(num4, mod_1024);
    std::cout << "Calculated    : " << product3.to_string() << std::endl;
    std::cout << "Expected      : 152642452731986236266917600130708390579028167712776439390699028344909777667204382863740435807987428154387474465444171074509230880947785368936452777958972785382354187915618459205899645333281563009066484139615965359723310359933667317792118326106773678447759884378243046264543382049899602593075709467366738045073" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num5.to_string() << std::endl;
    std::cout << "Number 2: " << num6.to_string() << std::endl;
    std::cout << "modulus : 2048bits" << std::endl;
    BigNumber product4 = num5.modMultiplication(num6, mod_2048);
    std::cout << "Calculated    : " << product4.to_string() << std::endl;
    std::cout << "Expected      : 11807412577886206517701956037499541833451608630036103153932863121864748179468771528470635261286916436986126745586181326265891311369979379226804543872017091210398196168810241590910459469708372556651533510441981841610120116907504758477898350210915176027207043720610825305430460843430311492933786691946485021746934432874990658302405065243632603783632645050709612435396654797181403159390121597112527955911313725285762258412684683062873934105118748059965709633232915218386369335945560999745041736792285306529907594911196719844470915947505877823182062210747177633153937411467602364789852041731702358509132538280547616365588" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num7.to_string() << std::endl;
    std::cout << "Number 2: " << num9.to_string() << std::endl;
    std::cout << "modulus : 1024bits" << std::endl;
    BigNumber product5 = num7.modMultiplication(num9, mod_1024);
    std::cout << "Calculated    : " << product5.to_string() << std::endl;
    std::cout << "Expected      : 152642452731986236266917600130708390579028167712776439390699028344909777667204382863740435807987428154387474465444171074509230880947785368936452777958972785382354187915618459205899645333281563009066484139615965359723310359933667317792118326106773678447759884378243046264543382049899602593075709467366738045073" << std::endl;

    // Inverse test cases

    std::cout << "===========================================================================================" << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num10.to_string() << std::endl;
    std::cout << "modulus : 512bits" << std::endl;
    BigNumber inverse1 = num10.modInverse(mod_512);
    std::cout << "Calculated    : " << inverse1.to_string() << std::endl;
    std::cout << "Expected      : 1" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num1.to_string() << std::endl;
    std::cout << "modulus : 512bits" << std::endl;
    BigNumber inverse2 = num1.modInverse(mod_512);
    std::cout << "Calculated    : " << inverse2.to_string() << std::endl;
    std::cout << "Expected      : 9147482900272338539705261019175300624143989181826741864596824711264600130384856868068663746785895787813079850763254056636409668114587263601063751447537857" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num3.to_string() << std::endl;
    std::cout << "modulus : 1024bits" << std::endl;
    BigNumber inverse3 = num3.modInverse(mod_1024);
    std::cout << "Calculated    : " << inverse3.to_string() << std::endl;
    std::cout << "Expected      : 83826255647348532952039310909632443629155926634631776688551386553928620201045045856336720761877400146188238198679761864440833140167835394214622070588397095718611497634307745942108675312507315990404512964298305295794003227565885465224735240684789907367780374383073291375803440961078028461817910818836871335125" << std::endl;
    
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num5.to_string() << std::endl;
    std::cout << "modulus : 2048bits" << std::endl;
    BigNumber inverse4 = num5.modInverse(mod_2048);
    std::cout << "Calculated    : " << inverse4.to_string() << std::endl;
    std::cout << "Expected      : 26938993354545034079281803459594062018862486039719702930194521037326251527747699261997015740574602841515136554623613750458405880001301400564831457912623225078289151338105357064035490909685536283972396012877146484999133447715644365259759780888070112408257323640553388919644969771584721493866498185401280530752670945938939211968887127984610030593693606163522597885310756487354874720529566844520670372006977723561029774249344620617944663222488601803479852980260113617818563026389583759072081422134974518885555546411904674098919287794693465404785926577761438116958072028437738359608603619541178410858170586946968829527203" << std::endl;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number 1: " << num11.to_string() << std::endl;
    std::cout << "modulus : 512bits" << std::endl;
    BigNumber inverse5 = num11.modInverse(mod_512);

    return 0;
}