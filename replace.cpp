#include <iostream>
#include <regex>
using namespace std;
int main() 
{
	string s = "{ \"revalidationTime\": \"2022-07-01T00:00:00+09:00\", \"sessRules\": { \"SESSRULE\": { \"authDefQos\": { \"arp\": { \"preemptVuln\": \"NOT_PREEMPTABLE\", \"preemptCap\": \"NOT_PREEMPT\", \"priorityLevel\": 3 }, \"5qi\": 1 }, \"sessRuleId\": \"SESSRULE\", \"authSessAmbr\": { \"downlink\": \"150 Mbps\", \"uplink\": \"150 Mbps\" } } }, \"qosDecs\": { \"400K\": { \"maxbrDl\": \"400 Kbps\", \"qosId\": \"400K\", \"maxbrUl\": \"400 Kbps\" } }, \"quotaNoti\": 0, \"zoneInfos\": \"0000000000\" }";
    std::cout << s << std::endl;
    std::cout << std::regex_replace(s, 
            std::regex("\"revalidationTime\"[a-zA-T\": 0-9-+]*[^,}]")
            , "\"revalidationTime\": \"0000-00-00T00:00:00+09:00\""
            ) << std::endl;
}
